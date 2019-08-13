//============================================================
//
//      Type:        simplePlot include file
//
//      Author:      Tommaso Bellosta
//                   Dipartimento di Scienze e Tecnologie Aerospaziali
//                   Politecnico di Milano
//                   Via La Masa 34, 20156 Milano, ITALY
//                   e-mail: tommaso.bellosta@polimi.it
//
//      Copyright:   2019, Tommaso Bellosta and the simplePlot contributors.
//                   This software is distributed under the MIT license, see LICENSE.txt
//
//============================================================

#ifndef GNUPLOT_GNUPLOTDRIVER_H
#define GNUPLOT_GNUPLOTDRIVER_H

#include <vector>
#include <string>
#include <fstream>

using namespace std;

enum gnuplot_action_type{

    GNUPLOT_PLOT,
    GNUPLOT_SAVE

};

enum gnuplot_save_type{

    GNUPLOT_PNG,
    GNUPLOT_EPS

};

/**
 * Class GnuplotDriver implements an handler for gnuplot to be called from c++ code.
 * Internally the class creates an input file that is used to run gnuplot.
 * As of now ONE object of the class can handle ONE 2D plot.
 *
 * See file src/main.cpp for some examples on how to use this library
 */
class GnuplotDriver {

private:
    gnuplot_action_type action; /** @todo driver can be called in "plot" mode or "save mode"**/
//    gnuplot_save_type saveType; /** \brief if action is GNUPLOT_SAVE plot will be exported in wanted format instead of being displayed**/
//    string saveName;            /** \brief if action is GNUPLOT_SAVE, file to be exported **/
    ofstream commandFile;       /**< \brief  fstream for gnuplot input file **/
    string plotOptions;         /**< \brief set plot options. Default is "with lines" **/


    /**
     * writes command in gnuplot input file GnuplotDriver::commandFile.
     * @param command string to be written to file
     */
    void write_command(const string& command);

public:
    GnuplotDriver(gnuplot_action_type action_type = GNUPLOT_PLOT);
    ~GnuplotDriver();

    // functions to set gnuplot properties. Must be called before GnuplotDriver::plot
    void setTitle(const string& title);                   /**< \brief sets title **/
    void setTitleFont(const int& size);                   /**< \brief sets title font size **/
    void setXRange(const double& x0, const double& x1);   /**< \brief sets range for x axis **/
    void setYRange(const double& y0, const double& y1);   /**< \brief sets range for y axis **/
    void setPlotOptions(const string& opts);              /**< \brief i.e. "with lines" **/

    void plot(const vector<double>& x, const vector<double>& y);
    void plot(const vector<double>& x0, const vector<double>& y0, const vector<double>& x1, const vector<double>& y1);


};


#endif //GNUPLOT_GNUPLOTDRIVER_H