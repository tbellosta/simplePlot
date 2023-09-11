//============================================================
//
//      Type:        simplePlot implementation file
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
#include <sys/types.h>
#include <sys/wait.h>

vector<vector<vector<double>>> GnuplotDriver::videoData = {};

GnuplotDriver::GnuplotDriver(gnuplot_axis_type axis, gnuplot_action_type action_type, string fileName, gnuplot_save_type format) {

    this->id = rand();
    this->commandFileName = "/tmp/gnuplotFile" + to_string(this->id) + ".txt";
    this->dataFileName = "/tmp/tmp_gnuplot_data" + to_string(this->id) + ".txt";

    this->action = action_type;

    this->commandFile.open(this->commandFileName,ios::trunc);

    this->plotOptions = " w l";

    if (fileName == "plot.png" && format == gnuplot_save_type::GNUPLOT_EPS) fileName = "plot.eps";

    this->saveName = fileName;
    this->saveType = format;

    if(this->action == gnuplot_action_type::GNUPLOT_SAVE) write_action_save();

    this->axisType = axis;
    switch (axisType) {
    case gnuplot_axis_type::GNUPLOT_XLOG:
      write_command("set logscale x");
      break;
    case gnuplot_axis_type::GNUPLOT_YLOG:
      write_command("set logscale y");
      break;
    case gnuplot_axis_type::GNUPLOT_LOGLOG:
      write_command("set logscale xy");
      break;
    }
}

GnuplotDriver::~GnuplotDriver() {

//    this->commandFile.close();

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

string GnuplotDriver::getTitle(const string& str){

    return " title \"" + str + "\"";

}

void GnuplotDriver::plot(const vector<double> &x, const vector<double> &y) {

    if(this->action == gnuplot_action_type::GNUPLOT_NONE){
        cout << "[WARNING] gnuplot action is set to GNUPLOT_NONE." << endl;
        return;
    }

    if(x.size() != y.size()){
        cout<<"\n\n[ERROR] x and y must have same dimension.\n\n"<<endl;
        throw std::runtime_error("void GnuplotDriver::plot(const vector<double> &x, const vector<double> &y)");
    }

    bool noLegend = false;
    if (this->legendTitles.empty()) {
        legendTitles = vector<string>(1);
        noLegend = true;
    }

    if(this->action != gnuplot_action_type::GNUPLOT_VIDEO) {
        // creates (tmp) data file
        ofstream tmp;
        tmp.open(this->dataFileName, ios::trunc);

        for (int i = 0; i < x.size(); ++i) {
            tmp << x[i] << " " << y[i] << endl;
        }

        tmp.close();

        if (noLegend) write_command("set nokey"); // hides legend
        write_command("plot \"" + this->dataFileName + "\"" + this->plotOptions + getTitle(this->legendTitles[0]));

        this->commandFile.close();

        // execute gnuplot
        executeGnuplot();
    }
    else{
        if(this->videoData.empty()) this->videoData = vector<vector<vector<double>>>(1);
        this->videoData[0].push_back(y);
    }

}

void GnuplotDriver::plot(const vector<double>& x0, const vector<double>& y0,
                         const vector<double>& x1, const vector<double>& y1) {

    if(this->action == gnuplot_action_type::GNUPLOT_NONE){
        cout << "[WARNING] gnuplot action is set to GNUPLOT_NONE." << endl;
        return;
    }

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

    bool noLegend = false;
    if (this->legendTitles.empty()) {
        legendTitles = vector<string>(2);
        noLegend = true;
    }

    if(this->action != gnuplot_action_type::GNUPLOT_VIDEO) {
        // creates (tmp) data file
        ofstream tmp;
        tmp.open(this->dataFileName, ios::trunc);

        for (int i = 0; i < x0.size(); ++i) {
            tmp << x0[i] << " " << y0[i] << " " << x1[i] << " " << y1[i] << endl;
        }

        tmp.close();

        if (noLegend) write_command("set nokey"); // hides legend
        write_command(
                "plot \"" + this->dataFileName + "\" u 1:2" + this->plotOptions + getTitle(legendTitles[0]) + ", '' u 3:4" + this->plotOptions + getTitle(legendTitles[1]));

        this->commandFile.close();


        // execute gnuplot
        executeGnuplot();
    }
    else{
        if(this->videoData.empty()) this->videoData = vector<vector<vector<double>>>(2);
        this->videoData[0].push_back(y0);
        this->videoData[1].push_back(y1);
    }

}

void GnuplotDriver::plot(const vector<double>& x0, const vector<double>& y0,
                         const vector<double>& x1, const vector<double>& y1,
                         const vector<double>& x2, const vector<double>& y2) {

    if(this->action == gnuplot_action_type::GNUPLOT_NONE){
        cout << "[WARNING] gnuplot action is set to GNUPLOT_NONE." << endl;
        return;
    }

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
    if(x2.size() != y2.size()){
        cout<<"\n\n[ERROR] x2 and y2 must have same dimension.\n\n"<<endl;
        throw std::runtime_error("void GnuplotDriver::plot(const vector<double>& x0, const vector<double>& y0, "
                                 "const vector<double>& x1, const vector<double>& y1, "
                                 "const vector<double>& x2, const vector<double>& y2)");
    }

    bool noLegend = false;
    if (this->legendTitles.empty()) {
        legendTitles = vector<string>(3);
        noLegend = true;
    }

    if(this->action != gnuplot_action_type::GNUPLOT_VIDEO) {
        // creates (tmp) data file
        ofstream tmp;
        tmp.open(this->dataFileName, ios::trunc);

        for (int i = 0; i < x0.size(); ++i) {
            tmp << x0[i] << " " << y0[i] << " " << x1[i] << " " << y1[i] << " " << x2[i] << " " << y2[i] << endl;
        }

        tmp.close();

        if (noLegend) write_command("set nokey"); // hides legend
        write_command(
                "plot \"" + this->dataFileName + "\" u 1:2" + this->plotOptions + getTitle(legendTitles[0]) + ", '' u 3:4" + this->plotOptions + getTitle(legendTitles[1]) + ", '' u 5:6" + this->plotOptions + getTitle(legendTitles[2]));

        this->commandFile.close();


        // execute gnuplot
        executeGnuplot();
    }
    else{
        if(this->videoData.empty()) this->videoData = vector<vector<vector<double>>>(3);
        this->videoData[0].push_back(y0);
        this->videoData[1].push_back(y1);
        this->videoData[2].push_back(y2);
    }

}

void GnuplotDriver::plot(const vector<double>& x0, const vector<double>& y0,
                         const vector<double>& x1, const vector<double>& y1,
                         const vector<double>& x2, const vector<double>& y2,
                         const vector<double>& x3, const vector<double>& y3) {

    if(this->action == gnuplot_action_type::GNUPLOT_NONE){
        cout << "[WARNING] gnuplot action is set to GNUPLOT_NONE." << endl;
        return;
    }

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
    if(x2.size() != y2.size()){
        cout<<"\n\n[ERROR] x2 and y2 must have same dimension.\n\n"<<endl;
        throw std::runtime_error("void GnuplotDriver::plot(const vector<double>& x0, const vector<double>& y0, "
                                 "const vector<double>& x1, const vector<double>& y1, "
                                 "const vector<double>& x2, const vector<double>& y2)");
    }
    if(x3.size() != y3.size()){
        cout<<"\n\n[ERROR] x3 and y3 must have same dimension.\n\n"<<endl;
        throw std::runtime_error("void GnuplotDriver::plot(const vector<double>& x0, const vector<double>& y0, "
                                 "const vector<double>& x1, const vector<double>& y1, "
                                 "const vector<double>& x2, const vector<double>& y2, "
                                 "const vector<double>& x3, const vector<double>& y3)");
    }

    bool noLegend = false;
    if (this->legendTitles.empty()) {
        legendTitles = vector<string>(4);
        noLegend = true;
    }

    if(this->action != gnuplot_action_type::GNUPLOT_VIDEO) {
        // creates (tmp) data file
        ofstream tmp;
        tmp.open(this->dataFileName, ios::trunc);

        for (int i = 0; i < x0.size(); ++i) {
            tmp << x0[i] << " " << y0[i] << " " << x1[i] << " " << y1[i] << " " << x2[i] << " " << y2[i] << " " << x3[i] << " " << y3[i] << endl;
        }

        tmp.close();

        if (noLegend) write_command("set nokey"); // hides legend
        write_command(
                "plot \"" + this->dataFileName + "\" u 1:2" + this->plotOptions + getTitle(legendTitles[0]) + ", '' u 3:4" + this->plotOptions + getTitle(legendTitles[1]) + ", '' u 5:6" + this->plotOptions + getTitle(legendTitles[2]) + ", '' u 7:8" + this->plotOptions + getTitle(legendTitles[3]));

        this->commandFile.close();


        // execute gnuplot
        executeGnuplot();
    }
    else{
        if(this->videoData.empty()) this->videoData = vector<vector<vector<double>>>(4);
        this->videoData[0].push_back(y0);
        this->videoData[1].push_back(y1);
        this->videoData[2].push_back(y2);
        this->videoData[3].push_back(y2);
    }

}

void GnuplotDriver::write_action_save() {

    switch(this->saveType){
    case gnuplot_save_type::GNUPLOT_EPS:
            write_command("set term epscairo");
            break;
    case gnuplot_save_type::GNUPLOT_PNG:
            write_command("set term png");
            break;
        default:
            cout<<"\n\n[ERROR] wrong output file type.\n\n"<<endl;
            throw std::runtime_error("void GnuplotDriver::write_action_save()");
    }

    write_command("set output \"" + this->saveName + "\"");

}

void GnuplotDriver::playAnimation(const vector<double> &x, const double &dt) {

    if(this->action != gnuplot_action_type::GNUPLOT_VIDEO){
        cout << "[WARNING] calling function GnuplotDriver::playAnimation\n"
                "but gnuplot action is not set to GNUPLOT_VIDEO." << endl;
        return;
    }

    ofstream tmp;
    tmp.open(this->dataFileName, ios::trunc);

    for (int i = 0; i < x.size(); ++i) {
        tmp << x[i] << " ";
        for (int j = 0; j < this->videoData[0].size(); ++j) {
            for (int k = 0; k < this->videoData.size(); ++k) {
                tmp << this->videoData[k][j][i] << " ";
            }
        }
        tmp << endl;
    }

    tmp.close();

    int nCurves = this->videoData.size();
    int nFrames = this->videoData[0].size();

    // find bounding box for computed data
    double min = 999;
    double max = -999;
    for (int i = 0; i < this->videoData[0].size(); ++i) {
        for (int j = 0; j < this->videoData[0][i].size(); ++j) {
            if (this->videoData[0][i][j] < min) min = this->videoData[0][i][j];
            if (this->videoData[0][i][j] > max) max = this->videoData[0][i][j];
        }
    }
    min -= (max > 0) ? (max*0.05) : (-max*0.05);
    max += (max > 0) ? (max*0.05) : (-max*0.05);

    write_command("set nokey");
    write_command("do for [t=2:" + to_string(nFrames+1) + "] {");
    write_command("set yrange [" + to_string(min) + " : " + to_string(max) + "]");
    if (nCurves == 1)
        write_command("plot \"" + this->dataFileName + "\" u 1:t" + this->plotOptions);
    else if (nCurves == 2)
        write_command("plot \"" + this->dataFileName + "\" u 1:2*t-2" + this->plotOptions + ", '' u 1:2*t-1" + this->plotOptions);

    write_command("pause " + to_string(dt));

    write_command("}");

    this->commandFile.close();

    // execute gnuplot
    executeGnuplot();
}

void GnuplotDriver::executeGnuplot() {

    pid_t child = fork();
    pid_t wpid;
    int status = 0;

    if (child < 0) {
        cout << "\n\n[ERROR] could not fork process.\n\n" << endl;
        throw std::runtime_error("void GnuplotDriver::plot(const vector<double>& x0, const vector<double>& y0,\n"
                                 "                         const vector<double>& x1, const vector<double>& y1)");
    } else if (child == 0) {
        // executes gnuplot
        execlp("gnuplot", "gnuplot", this->commandFileName.c_str(), "--persist", (char *) NULL);
    } else {
        //main, wait for child
        while ((wpid = wait(&status)) > 0);
    }
}

void GnuplotDriver::setLegendTitles(const vector<string>& ss){

    this->legendTitles = ss;

}
void GnuplotDriver::setAxisType(const gnuplot_axis_type &axis) {

    axisType = axis;

}
