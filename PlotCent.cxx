#include "TSystem.h"
#include "PlotManager.h"
#include "PlotManager.cxx"

void PlotCent() {
    PlotManager* pm = new PlotManager();

    const int nf = 7; // number of tfiles
    const int nc = 7; // number of cumulants

    const char* fNames[nf] = {
        "/Path-to-UrQMD/y0p5.root",
        "/Path-to-BES1/stat/y0p5.root",
        "/Path-to-BES1/sys/y0p5.root",
        "/Path-to-BES2/stat/RefMult3_y0p5.root",
        "/Path-to-BES2/sys/RefMult3_y0p5.root",
        "/Path-to-BES2/stat/RefMult3X_y0p5.root",
        "/Path-to-BES2/sys/RefMult3X_y0p5.root"
    };

    int mcolor[nf] = {
        kGreyYellow,
        kBlack, kBlack,
        kRed, kRed,
        kOrange, kOrange
    };
    
    int mstyle[nf] = {
        21, 30, 30, 20, 20, 21, 21
    };
    double msize[nf] = {
        1.3, 2.2, 2.2, 1.3, 1.3, 1.3, 1.3
    };
    const char* pstyle[nf] = {
        "e3"
        "epl", "e[]", 
        "epl", "e[]", 
        "epl", "e[]"
    };

    const char* sname = "Centpdf";

    TCanvas* cover = new TCanvas();
    cover->Print(Form("%s(", sname));

    TLatex* lat = new TLatex();
    lat->SetTextColor(2);
    lat->SetTextFont(64);
    lat->SetTextSize(12);

    double Npart[9] = {
        339, 289, 224, 158, 109, 71, 44, 25, 13
    };

    sMode_t sMode = 0; 
    for (cMode_t cMode=0; cMode<1; cMode++) {
        for (pMode_t pMode=0; pMode<3; pMode++) {
            if (cMode == 1 && pMode == 0) { continue; }
            pm->Init(nf, fNames, mcolor, mstyle, msize, pstyle, pMode, cMode, sMode);
            pm->SetDefaultLabelLocation();
            pm->ReadGraph();
            for (int i=0; i<nf; i++) { // if some of the files are using other Npart, should not use this loop. Set them separately instead!
                pm->SetGraphXArray(9, 0, Npart);
            }
            pm->SetNp(9);
            pm->SetUrQMD(0);
            // pm->SetLabelLocationX(5, 0.675); 
            pm->SetLegendN(4);
            pm->SetLegend(0, 0, "UrQMD-I", "p");
            pm->SetLegend(1, 1, "BES-I", "ep");
            pm->SetLegend(2, 3, "BES-II Corr. (RefMult3)", "ep");
            pm->SetLegend(3, 5, "BES-II Corr. (RefMult3X)", "ep");
            pm->Plot();
            pm->AddLegend("14.6", 0.5);

            lat->DrawLatexNDC(0.75, 0.04, "[]: Systematic uncertainty");
            pm->Save(sname);
        }
    }

    cover->Print(Form("%s)", sname));

}
