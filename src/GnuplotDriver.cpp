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

vector<vector<vector<double>>> GnuplotDriver::videoData = {};

GnuplotDriver::GnuplotDriver(gnuplot_action_type action_type, string fileName, gnuplot_save_type format) {

    this->id = rand();
    this->commandFileName = "/tmp/gnuplotFile" + to_string(this->id) + ".txt";
    this->dataFileName = "/tmp/tmp_gnuplot_data" + to_string(this->id) + ".txt";

    this->action = action_type;

    this->commandFile.open(this->commandFileName,ios::trunc);

    this->plotOptions = " w l";

    if (fileName == "plot.png" && format == GNUPLOT_EPS) fileName = "plot.eps";

    this->saveName = fileName;
    this->saveType = format;

    if(this->action == GNUPLOT_SAVE) write_action_save();

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

void GnuplotDriver::plot(const vector<double> &x, const vector<double> &y) {

    if(x.size() != y.size()){
        cout<<"\n\n[ERROR] x and y must have same dimension.\n\n"<<endl;
        throw std::runtime_error("void GnuplotDriver::plot(const vector<double> &x, const vector<double> &y)");
    }

    if(this->action != GNUPLOT_VIDEO) {
        // creates (tmp) data file
        ofstream tmp;
        tmp.open(this->dataFileName, ios::trunc);

        for (int i = 0; i < x.size(); ++i) {
            tmp << x[i] << " " << y[i] << endl;
        }

        tmp.close();

        write_command("set nokey"); // hides legend
        write_command("plot \"" + this->dataFileName + "\"" + this->plotOptions);

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

    if(this->action != GNUPLOT_VIDEO) {
        // creates (tmp) data file
        ofstream tmp;
        tmp.open(this->dataFileName, ios::trunc);

        for (int i = 0; i < x0.size(); ++i) {
            tmp << x0[i] << " " << y0[i] << " " << x1[i] << " " << y1[i] << endl;
        }

        tmp.close();

        write_command("set nokey"); // hides legend
        write_command(
                "plot \"" + this->dataFileName + "\" u 1:2" + this->plotOptions + ", '' u 3:4" + this->plotOptions);

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

void GnuplotDriver::write_action_save() {

    switch(this->saveType){
        case GNUPLOT_EPS:
            write_command("set term epscairo");
            break;
        case GNUPLOT_PNG:
            write_command("set term png");
            break;
        default:
            cout<<"\n\n[ERROR] wrong output file type.\n\n"<<endl;
            throw std::runtime_error("void GnuplotDriver::write_action_save()");
    }

    write_command("set output \"" + this->saveName + "\"");

}

void GnuplotDriver::playAnimation(const vector<double> &x, const double &dt) {

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

    write_command("set nokey");
    write_command("do for [t=2:" + to_string(nFrames+1) + "] {");
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
