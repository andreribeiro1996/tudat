/*    Copyright (c) 2010-2018, Delft University of Technology
 *    All rigths reserved
 *
 *    This file is part of the Tudat. Redistribution and use in source and
 *    binary forms, with or without modification, are permitted exclusively
 *    under the terms of the Modified BSD license. You should have received
 *    a copy of the license with this file. If not, please or visit:
 *    http://tudat.tudelft.nl/LICENSE.
 */

#include "Tudat/SimulationSetup/PropagationSetup/createStateDerivativeModel.h"
#include "Tudat/Astrodynamics/MissionSegments/lambertTargeter.h"
#include "Tudat/Astrodynamics/MissionSegments/lambertTargeterIzzo.h"
#include "Tudat/Astrodynamics/MissionSegments/lambertRoutines.h"
#include "Tudat/SimulationSetup/EnvironmentSetup/defaultBodies.h"
#include "Tudat/SimulationSetup/PropagationSetup/createAccelerationModels.h"
#include <Tudat/SimulationSetup/tudatEstimationHeader.h>

#include "Tudat/Astrodynamics/Gravitation/librationPoint.h"
#include "Tudat/Astrodynamics/BasicAstrodynamics/celestialBodyConstants.h"
#include "Tudat/Astrodynamics/BasicAstrodynamics/missionGeometry.h"

#include "Tudat/SimulationSetup/PropagationSetup/fullPropagationLambertTargeter.h"

// required for the MGA
#include "Tudat/Astrodynamics/Ephemerides/approximatePlanetPositions.h"
#include "Tudat/Astrodynamics/TrajectoryDesign/trajectory.h"
#include "Tudat/Astrodynamics/TrajectoryDesign/exportTrajectory.h"
#include "Tudat/Astrodynamics/TrajectoryDesign/planetTrajectory.h"

#include "Tudat/SimulationSetup/PropagationSetup/propagationPatchedConicFullProblem.h"

namespace tudat
{

namespace propagators
{

void fullPropagationMGA(
        simulation_setup::NamedBodyMap& bodyMap,
        const basic_astrodynamics::AccelerationMap& accelerationMap,
        const int numberOfLegs,
        const std::vector< std::string >& transferBodyOrder,
        const std::vector< std::string >& bodiesAndManoeuvresOrder,
        const std::vector< std::string >& centralBody,
        const std::vector< std::string >& bodyToPropagate,
        const std::vector< transfer_trajectories::TransferLegType>& legTypeVector,
        const Eigen::VectorXd& trajectoryVariableVector,
        const Eigen::VectorXd& minimumPericenterRadiiVector,
        const Eigen::VectorXd& semiMajorAxesVector,
        const Eigen::VectorXd& eccentricitiesVector,
        const std::shared_ptr< numerical_integrators::IntegratorSettings< double > > integratorSettings,
        std::map< int, std::map< double, Eigen::Vector6d > >& lambertTargeterResultForEachLeg,
        std::map< int, std::map< double, Eigen::Vector6d > >& fullProblemResultForEachLeg){


    // calculate the patched conic trajectory from the body map
    transfer_trajectories::Trajectory trajectory = propagators::createTransferTrajectoryObject(
            bodyMap, transferBodyOrder, centralBody[0], legTypeVector, utilities::convertEigenVectorToStlVector(trajectoryVariableVector),
            utilities::convertEigenVectorToStlVector(minimumPericenterRadiiVector), true, semiMajorAxesVector[0], eccentricitiesVector[0],
            true, semiMajorAxesVector[1], eccentricitiesVector[1]);


    int numberLegsIncludingDSM = ((trajectoryVariableVector.size()-1-numberOfLegs)/4.0) + numberOfLegs ;

    std::vector< Eigen::Vector3d > positionVector;
    std::vector< double > timeVector;
    std::vector< double > deltaVVector;
    double totalDeltaV;


    std::map< double, std::pair<Eigen::Vector6d, Eigen::Vector6d> > stateDifferenceAtDepartureAndArrival;

    // Calculate the orbits
    trajectory.calculateTrajectory( totalDeltaV );
    trajectory.maneuvers( positionVector, timeVector, deltaVVector );

    std::map< int, Eigen::Vector3d > cartesianPositionAtDepartureLambertTargeter;
    std::map< int, Eigen::Vector3d > cartesianPositionAtArrivalLambertTargeter;


    double timeOfFlight;
    int counterLegTotal = 0;
    int counterLegWithDSM = 0;
    std::vector< double > timeOfFlightVector;



    // Calculate the time of flight for each leg (one leg with a deep-space manoeuvre is divided into two sub-legs)

    for (int i = 0 ; i < numberOfLegs - 1 ; i ++){

        if (legTypeVector[i] == transfer_trajectories::mga_Departure ||
                legTypeVector[i] == transfer_trajectories::mga_Swingby ){

            timeOfFlight = trajectoryVariableVector[1 + counterLegTotal];
            timeOfFlightVector.push_back( timeOfFlight );
            counterLegTotal++;

        }

        else {

                timeOfFlight = trajectoryVariableVector[numberOfLegs + 1 + counterLegWithDSM * 4]
                        * trajectoryVariableVector[counterLegTotal + 1];
                timeOfFlightVector.push_back( timeOfFlight );

                timeOfFlight = (1 - trajectoryVariableVector[numberOfLegs + 1 + counterLegWithDSM * 4])
                        * trajectoryVariableVector[counterLegTotal + 1];
                timeOfFlightVector.push_back( timeOfFlight );
                counterLegWithDSM++;
                counterLegTotal++;
        }

    }



    for (int i = 0; i<numberLegsIncludingDSM-1 ; i++)
    {

        cartesianPositionAtDepartureLambertTargeter[ i ] = positionVector[i];
        cartesianPositionAtArrivalLambertTargeter[ i ] = positionVector[i+1];

        std::vector< std::string > departureAndArrivalBodies;
        departureAndArrivalBodies.push_back( bodiesAndManoeuvresOrder[i] );
        departureAndArrivalBodies.push_back( bodiesAndManoeuvresOrder[1 + i]);
        std::cout << "departure and arrival bodies: " << departureAndArrivalBodies[0] << "\n\n";
        std::cout << "departure and arrival bodies: " << departureAndArrivalBodies[1] << "\n\n";


        Eigen::Vector3d cartesianPositionAtDeparture = cartesianPositionAtDepartureLambertTargeter[i];
        Eigen::Vector3d cartesianPositionAtArrival = cartesianPositionAtArrivalLambertTargeter[i];



        double initialTime = 0.0;

       // Compute the difference in state between the full problem and the Lambert targeter solution at departure and at arrival
        std::map< double, Eigen::Vector6d > lambertTargeterResultForOneLeg;
        std::map< double, Eigen::Vector6d > fullProblemResultForOneLeg;
        propagateLambertTargeterAndFullProblem( cartesianPositionAtDeparture, cartesianPositionAtArrival,
                timeOfFlightVector[i], initialTime, bodyMap, accelerationMap, bodyToPropagate, centralBody,
                integratorSettings, lambertTargeterResultForOneLeg, fullProblemResultForOneLeg,
                departureAndArrivalBodies, false, false);


        lambertTargeterResultForEachLeg[i] = lambertTargeterResultForOneLeg;
        fullProblemResultForEachLeg[i] = fullProblemResultForOneLeg;


    }


}




std::map< int, std::pair< Eigen::Vector6d, Eigen::Vector6d > > getDifferenceFullPropagationWrtLambertTargeterMGA(
        simulation_setup::NamedBodyMap& bodyMap,
        const basic_astrodynamics::AccelerationMap& accelerationMap,
        const int numberOfLegs,
        const std::vector< std::string >& transferBodyOrder,
        const std::vector< std::string >& bodiesAndManoeuvresOrder,
        const std::vector< std::string >& centralBody,
        const std::vector< std::string >& bodyToPropagate,
        const std::vector< transfer_trajectories::TransferLegType >& legTypeVector,
        const Eigen::VectorXd& trajectoryVariableVector,
        const Eigen::VectorXd& minimumPericenterRadiiVector,
        const Eigen::VectorXd& semiMajorAxesVector,
        const Eigen::VectorXd& eccentricitiesVector,
        const std::shared_ptr< numerical_integrators::IntegratorSettings< double > > integratorSettings)
{

    int numberLegsIncludingDSM = ((trajectoryVariableVector.size()-1-numberOfLegs)/4.0) + numberOfLegs ;


    std::map< int, std::map< double, Eigen::Vector6d > > lambertTargeterResultForEachLeg;
    std::map< int, std::map< double, Eigen::Vector6d > > fullProblemResultForEachLeg;

    // compute full problem and Lambert targeter solution both at departure and arrival.

      fullPropagationMGA(bodyMap, accelerationMap, numberOfLegs, transferBodyOrder, bodiesAndManoeuvresOrder, centralBody, bodyToPropagate, legTypeVector,
                       trajectoryVariableVector, minimumPericenterRadiiVector, semiMajorAxesVector, eccentricitiesVector, integratorSettings,
                         lambertTargeterResultForEachLeg, fullProblemResultForEachLeg);


    std::map< int, std::pair< Eigen::Vector6d, Eigen::Vector6d > > stateDifferenceAtArrivalAndDepartureForEachLeg;

    for (int i = 0 ; i< numberLegsIncludingDSM-1 ; i++){

        std::map< double, Eigen::Vector6d > lambertTargeterResultCurrentLeg = lambertTargeterResultForEachLeg[i];
        std::map< double, Eigen::Vector6d > fullProblemResultCurrentLeg = fullProblemResultForEachLeg[i];

        Eigen::Vector6d stateLambertTargeterAtDepartureForOneLeg = lambertTargeterResultCurrentLeg.begin( )->second;
        Eigen::Vector6d stateFullProblemAtDepartureForOneLeg = fullProblemResultCurrentLeg.begin( )->second;
        Eigen::Vector6d stateLambertTargeterAtArrivalForOneLeg = lambertTargeterResultCurrentLeg.rbegin( )->second;
        Eigen::Vector6d stateFullProblemAtArrivalForOneLeg = fullProblemResultCurrentLeg.rbegin( )->second;

        stateDifferenceAtArrivalAndDepartureForEachLeg[i] = std::make_pair( stateLambertTargeterAtDepartureForOneLeg -
                                                                            stateFullProblemAtDepartureForOneLeg,
                                                                            stateLambertTargeterAtArrivalForOneLeg -
                                                                            stateFullProblemAtArrivalForOneLeg);



    }


    return stateDifferenceAtArrivalAndDepartureForEachLeg;

}


}

}
