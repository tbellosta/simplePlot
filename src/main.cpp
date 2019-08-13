//============================================================
//
//      Type:        simplePlot example file
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

using namespace std;

int main(){

    GnuplotDriver plt;

    plt.setTitle("prova");
    plt.setTitleFont(20);

    vector<double> tmp{0,1,2,3,4,5,6,7,8,9,10};
    vector<double> tmp1{10,11,12,13,14,15,16,17,18,19,20};
    plt.plot(tmp,tmp,tmp,tmp1);

}