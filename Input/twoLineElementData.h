/*! \file twoLineElementsData.h
 *    This header file defines a class which stores Two-Line Element (TLE) data
 *    read by the TwoLineElementsTextFileReader.
 *
 *    Path              : /Input/
 *    Version           : 5
 *    Check status      : Checked
 *
 *    Author            : J. Leloux
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : j.leloux@tudelft.nl, j.leloux@student.tudelft.nl,
 *                        j.leloux@gmail.com
 *
 *    Checker           : K. Kumar
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : K.Kumar@tudelft.nl
 *
 *    Date created      : 4 March, 2011
 *    Last modified     : 8 August, 2011
 *
 *    References
 *      Literature research including information on TLE's:
 *      J. Leloux (2010), Filtering Techniques for Orbital Debris Conjunction
 *                        Analysis - applied to SSN TLE catalog data and
 *                        including astrodynamics and collision probability
 *                        theory.
 *      Program to download clean TLE's:
 *                        http://celestrak.com/SpaceTrack/TLERetrieverHelp.asp.
 *      TLE format explanation:
 *                        http://www.space-track.org/tle_format.html
 *                        http://celestrak.com/columns/v04n03/
 *                        http://celestrak.com/NORAD/documentation/tle-fmt.asp.
 *
 *    Notes
 *
 *    Copyright (c) 2010 Delft University of Technology.
 *
 *    This software is protected by national and international copyright.
 *    Any unauthorized use, reproduction or modification is unlawful and
 *    will be prosecuted. Commercial and non-private application of the
 *    software in any form is strictly prohibited unless otherwise granted
 *    by the authors.
 *
 *    The code is provided without any warranty; without even the implied
 *    warranty of merchantibility or fitness for a particular purpose.
 *
 *    Changelog
 *      YYMMDD    Author            Comment
 *      110304    J. Leloux         First setup of TLE data class.
 *      110415    K. Kumar          Minor corrections.
 *      110722    J. Leloux         Added variables and changed variables into
 *                                  the KeplerianElements class.
 *      110802    K. Kumar          Minor changes.
 *      110802    J. Leloux         Minor correction and checked for compliance
 *                                  with TLEreader in rev 138.
 */

#ifndef TWOLINEELEMENTDATA_H
#define TWOLINEELEMENTDATA_H

// Include statements.
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include "keplerianElements.h"

// Using declarations.
using std::vector;
using std::string;

//! TLE data class.
/*!
 * Class containing all variables of one TLE for one space debris
 * object, according to the above format definition.
 * See reference for explanation of the variables.
 */
class TwoLineElementData
{
public:

    //! Default constructor.
    /*!
     * Default constructor.
     */
    TwoLineElementData( );

    //! Default destructor.
    /*!
     * Default destructor.
     */
    ~TwoLineElementData( );

    // Line 0 strings.

    //! Object name string vector.
    /*!
     * Vector containing the separate words of the name of the object as strings,
     * range could go as far as 10 strings containing parts of the name.
     */
    vector< string > objectName;

    //! Object name string.
    /*!
     * String containing the name of the object,
     */
    string objectNameString;

    // Line 1 variables.

    //! Line number of line 1.
    /*!
     * Line number of line 1, always '1'.
     */
    unsigned int lineNumberLine1;

    //! Object identfication number.
    /*!
     * The object identfication number, range 0 to 99999.
     */
    unsigned int objectIdentificationNumber;

    //! TLE classification.
    /*!
     * TLE classification, U is unclassified.
     */
    char tleClassification;

    //! Launch year two digits.
    /*!
     * Launch year, two digits, 57-99 (1900s) and 00-56 (2000s).
     */
    unsigned int launchYear;

    //! Launch year four digits.
    /*!
     * Launch year, four digits, converted from launchYear,
     * range 1957 to present year.
     */
    unsigned int fourDigitlaunchYear;

    //! Launch number.
    /*!
     * Launch number of above launch year, range 0 to 999.
     */
    unsigned int launchNumber;

    //! Part of the launch.
    /*!
     * Part/piece/fragment of the launch, range AAA - ZZZ.
     */
    string launchPart;

    //! TLE epoch year two digits.
    /*!
     * TLE epoch year, two digits, 57-99 (1900s) and 00-56 (2000s).
     */
    unsigned int epochYear;

    //! TLE epoch year four digits.
    /*!
     * TLE epoch year, four digits, converted from epochYear, range 1957 to
     * present year.
     */
    unsigned int fourDigitEpochYear;

    //! Epoch day of the year.
    /*!
     * Epoch day of the year as a double, range 0.0 to about 367.0.
     */
    double epochDay;

    //! First derivative of the mean motion divided by two.
    /*!
     * First derivative of the mean motion divided by two, in units of
     * revolutions per day squared.
     */
    double firstDerivativeOfMeanMotionDividedByTwo;

    //! Coefficient of second derivative of the mean motion divided by six.
    /*!
     * Coefficient of scientific notation of the second derivative of the mean
     * motion divided by six.
     */
    double coefficientOfSecondDerivativeOfMeanMotionDividedBySix;

    //! Exponent of second derivative of the mean motion divided by six.
    /*!
     * Exponent of scientific notation of the second derivative of the mean
     * motion divided by six.
     */
    double exponentOfSecondDerivativeOfMeanMotionDividedBySix;

    //! Second derivative of the mean motion divided by six.
    /*!
     * The second derivative of the mean motion divided by six, in units of
     * revolutions per day cubed.
     */
    double secondDerivativeOfMeanMotionDividedBySix;

    //! Coefficient of B* (bStar) drag term.
    /*!
     * Coefficient of scientific notation of B* (bStar) drag term.
     */
    double coefficientOfBStar;

    //! Exponent of B* (bStar) drag term.
    /*!
     * Exponent of scientific notation of B* (bStar) drag term.
     */
    int exponentOfBStar;

    //! B* (bStar) drag term.
    /*!
     * B* (bStar) drag term, in units of inverse Earth radius.
     */
    double bStar;

    //! Orbital model.
    /*!
     * Orbital model, always '0' nowadays (SGP4/SDP4).
     */
    unsigned int orbitalModel;

    //! TLE number.
    /*!
     * TLE number, range 0 to 9999, loops back to 0 when 9999 is reached.
     */
    unsigned int tleNumber;

    //! Modulo-10 checksum of line 1.
    /*!
     * Modulo-10 checksum of TLE line 1, all integers except for the
     * modulo-10 checksum integer are added, minus signs count as one,
     * rest is ignored.
     */
    unsigned int modulo10CheckSumLine1;

    // Line 2 variables.

    //! Line number of line 2.
    /*!
     * Line number of line 2, always '2'.
     */
    unsigned int lineNumberLine2;

    //! Object identification number of line 2.
    /*!
     * Object identification number of line 2, same as the one of line 1.
     */
    unsigned int objectIdentificationNumberLine2;

    //! TLE Keplerian Elements.
    /*!
     * Including:
     * Inclination of the object, ranging between 0 and 180 degrees.
     * Right ascension of ascending node, ranging between 0 and 360 degrees.
     * Eccentricity, ranging between 0 and 180 degrees,
     * Argument of Perigee, ranging between 0 and 360 degrees.
     * Semi-Major Axis, calculated from the other TLE variables.
     */
    KeplerianElements TLEKeplerianElements;

    //! Mean anomaly.
    /*!
     *  Mean anomaly, range between 0 and 360 degrees
     */
    double meanAnomaly;

    //! Mean motion.
    /*!
     *  Mean motion, in revolutions per day.
     */
    double meanMotion;

    //! Revolution number.
    /*!
     * Revolution number of the object, range 0 to 99999,
     * loops from 99999 back to 0.
     */
    unsigned int revolutionNumber;

    //! Total revolution number.
    /*!
     * Total revolution number calculated with mean motion.
     */
    unsigned int totalRevolutionNumber;

    //! Modulo-10 checksum of line 2.
    /*!
     * Modulo-10 checksum of TLE line 2, all integers except for the
     * modulo-10 checksum integer are added, rest is ignored.
     */
    unsigned int modulo10CheckSumLine2;

    //! Perigee of object.
    /*!
     * Perigee of the object is calculated from the other TLE variables.
     */
    double perigee;

    //! Apogee of object.
    /*!
     * Apogee of the object is calculated from the other TLE variables.
     */
    double apogee;

    //! Original line numbers of the TLE.
    /*!
     * The line numbers of this TLE in the original input file are saved here.
     */
    vector< int > lineNumbers;

    //! Overloaded ostream to print class information.
    /*!
     *  Overloaded ostream to print class information; prints all
     *  converted TLE variables obtained from TLE.
     */
    friend std::ostream& operator<<( std::ostream& stream,
                                     TwoLineElementData& twoLineElementData );

protected:

private:
};

#endif // TWOLINEELEMENTDATA_H

// End of file.