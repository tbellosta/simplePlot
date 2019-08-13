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

#include "GnuplotDriver.h"
#include <iostream>
#include <execinfo.h>
#include <unistd.h>

GnuplotDriver::GnuplotDriver(gnuplot_action_type action_type) {

    this->action = action_type;

    this->commandFile.open("/tmp/gnuplotFile.txt",ios::trunc);

    this->plotOptions = " w l";

}

GnuplotDriver::~GnuplotDriver() {

    this->commandFile.close();

}

void GnuplotDriver::write_command(const string& command) {

    this->commandFile << command << endl;
}

void GnuplotDriver::setTitle(const string &title) {

    write_command("set title \"" + title + "\"");

}

void GnuplotDriver::setXRange(const double &x0, const double &x1) {

    write_command("set xrange [" + to_string(x0) + ":" + to_string(x1) + "]");

}

void GnuplotDriver::setYRange(const double &y0, const double &y1) {

    write_command("set xrange [" + to_string(y0) + ":" + to_string(y1) + "]");

}

void GnuplotDriver::setPlotOptions(const string &opts) {

    this->plotOptions = " " + opts;

}

void GnuplotDriver::setTitleFont(const int &size) {

    write_command("set title  font \"," + to_string(size) + "\"");

}

void GnuplotDriver::plot(const vector<double> &x, const vector<double> &y) {

    if(x.size() != y.size()){
        cout<<"\n\n[ERROR] x and y must have same dimension.\n\n"<<endl;
        throw std::runtime_error("void GnuplotDriver::plot(const vector<double> &x, const vector<double> &y)");
    }

    // creates (tmp) data file
    ofstream tmp;
    tmp.open("/tmp/tmp_gnuplot_data.txt", ios::trunc);

    for (int i = 0; i < x.size(); ++i) {
        tmp << x[i] << " " << y[i] <<endl;
    }

    tmp.close();

    write_command("set nokey"); // hides legend
    write_command("plot \"/tmp/tmp_gnuplot_data.txt\"" + this->plotOptions);

    // executes gnuplot
    execlp("gnuplot","gnuplot","/tmp/gnuplotFile.txt","--persist", (char*) NULL);

}

void GnuplotDriver::plot(const vector<double>& x0, const vector<double>& y0,
                         const vector<double>& x1, const vector<double>& y1) {

    if(x0.size() != y0.size()){
        cout<<"\n\n[ERROR] x0 and y0 must have same dimension.\n\n"<<endl;
        throw std::runtime_error("void GnuplotDriver::plot(const vector<double>& x0, const vector<double>& y0, "
                                 "const vector<double>& x1, const vector<double>& y1)");
    }
    if(x1.size() != y1.size()){
        cout<<"\n\n[ERROR] x1 and y1 must have same dimension.\n\n"<<endl;
        throw std::runtime_error("void GnuplotDriver::plot(const vector<double>& x0, const vector<double>& y0, "
                                 "const vector<double>& x1, const vector<double>& y1)");
    }

    // creates (tmp) data file
    ofstream tmp;
    tmp.open("/tmp/tmp_gnuplot_data.txt", ios::trunc);

    for (int i = 0; i < x0.size(); ++i) {
        tmp << x0[i] << " " << y0[i] << " " << x1[i] << " " << y1[i] <<endl;
    }

    tmp.close();

    write_command("set nokey"); // hides legend
    write_command("plot \"/tmp/tmp_gnuplot_data.txt\" u 1:2" + this->plotOptions + ", '' u 3:4" + this->plotOptions);

    // executes gnuplot
    execlp("gnuplot","gnuplot","/tmp/gnuplotFile.txt","--persist", (char*) NULL);

}
