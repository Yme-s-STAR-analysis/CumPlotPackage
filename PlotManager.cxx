#include <iostream>
#include "TFile.h"
#include "TStyle.h"
#include "TGraphErrors.h"
#include "TString.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLatex.h"
#include "TLine.h"
#include "TLegend.h"
#include "TH1D.h"
#include "TSystem.h"
#include "TROOT.h"
#include "stdlib.h"
#include "PlotManager.h"
#include "CanvasTools.h"
#include "GraphCollection.h"

PlotManager::PlotManager() {
    c = new TCanvas("cas", "", 1200, 500);
    gcll = new GraphCollection();
    tl = new TLine(x1, 1, x2, 1);
    tl->SetLineColor(2);
    tl->SetLineStyle(2);
    share_lat = new TLatex();
}

void PlotManager::Init(int nfiles, const char** fnames, int* mcolor, int* mstyle, double* msize, const char** pstyle, pMode_t pMode, cMode_t cMode, sMode_t sMode) {
    nf = nfiles;
    for (int i=0; i<nf; i++) {
        std::cout << "[LOG] - Init: Loading File: " << fnames[i] << std::endl;
        tfs[i] = new TFile(fnames[i]);
        this->mcolor[i] = mcolor[i];
        this->mstyle[i] = mstyle[i];
        this->msize[i] = msize[i];
        this->pstyle[i] = const_cast<char*>(pstyle[i]);
    }
    if (pMode == 0) {
        pName = const_cast<char*>("Netp");
    } else if (pMode == 1) {
        pName = const_cast<char*>("Pro");
    } else if (pMode == 2) {
        pName = const_cast<char*>("Pbar");
    } else {
        std::cout << "[ERROR] - Init: Invalid pMode.\n";
        exit(-1);
    }
    this->pMode = pMode;
    if (cMode < 0 || cMode > 1) {
        std::cout << "[ERROR] - Init: Invalid cMode.\n";
        exit(-1);
    }
    this->cMode = cMode;
    if (sMode == 0) {
        x1 = -20;
        x2 = 390;
    } else if (sMode == 1) {
        x1 = -0.1;
        // x2 = 0.9;
        x2 = 0.7;
    } else if (sMode == 2) {
        x1 = 0.1;
        x2 = 0.6;
    } else if (sMode == 3) {
        x1 = 0.65;
        x2 = 2.15;
    } else {
        std::cout << "[ERROR] - Init: Invalid sMode.\n";
        exit(-1);
    }
    this->sMode = sMode;
    c->Clear();
    SetDefaultLabelLocation();
    share_lat->SetTextFont(23);

    legacy = false;
}

// legacy = false: new labels
void PlotManager::Init(int nfiles, const char** fnames, int* mcolor, int* mstyle, double* msize, const char** pstyle, pMode_t pMode, cMode_t cMode, sMode_t sMode, bool legacy) {
    Init(nfiles, fnames, mcolor, mstyle, msize, pstyle, pMode, cMode, sMode);
    this->legacy = legacy;
}

void PlotManager::SetLatexMode(bool des) {
    if (des) { // for description
        share_lat->SetTextSize(23);
    } else { // for labels (x/y)
        share_lat->SetTextSize(28);
    }
}

void PlotManager::SetDefaultLabelLocation() {
    labelX[0] = 0.100;
    labelX[1] = 0.100;
    labelX[2] = 0.330;
    labelX[3] = 0.330;
    labelX[4] = 0.675;
    labelX[5] = 0.595;
    labelX[6] = 0.820;
    labelY[0] = 0.922;
    labelY[1] = 0.482;
    labelY[2] = 0.922;
    labelY[3] = 0.482;
    labelY[4] = 0.922;
    labelY[5] = 0.482;
    labelY[6] = 0.922;
}

void PlotManager::SetLabelLocationX(int i, double x) {
    labelX[i] = x;
}

void PlotManager::SetLabelLocationY(int i, double x) {
    labelY[i] = x;
}

void PlotManager::SetLabelLocation(int i, double x, double y) {
    labelX[i] = x;
    labelY[i] = y;
}

void PlotManager::ReadGraph() {
    const char* tgnames[2][nc] = {
        {"C1", "C3", "C2", "C4", "R21", "R32", "R42"},
        {"k1", "k3", "k2", "k4", "k21", "k31", "k41"}
    };
    for (int i=0; i<nf; i++) {
        for (int j=0; j<nc; j++) {
            if (pMode == 0 && j > 6) { break; }
            if (!legacy && pMode == 0 && cMode == 0 && j == 4) { // netp, R21->R2s
                tfs[i]->GetObject(
                    Form("%s_R2s", pName), tgs[i][j]
                );
            } else if (!legacy && cMode == 0 && j == 5) { // any, R21->R2s
                tfs[i]->GetObject(
                    Form("%s_R31", pName), tgs[i][j]
                );
            } else {
                tfs[i]->GetObject(
                    Form("%s_%s", pName, tgnames[cMode][j]), tgs[i][j]
                );
            }
            tgs[i][j]->SetMarkerColor(mcolor[i]);
            tgs[i][j]->SetLineColor(mcolor[i]);
            tgs[i][j]->SetMarkerStyle(mstyle[i]);
            tgs[i][j]->SetMarkerSize(msize[i]);
        }
    }
}

void PlotManager::SetGraphXArray(int n, int ifile, double* x) {
    for (int j=0; j<nc; j++) {
        for (int k=0; k<n; k++) {
            if (tgs[ifile][j]->GetN() != 0) {
                tgs[ifile][j]->SetPointX(k, x[k]);
            }
        }
    }
}

void PlotManager::SetUrQMD(int idx) {
    for (int i=0; i<nc; i++) {
        for (int j=0; j<np; j++) { 
            tgs[idx][i]->SetPointError(j, 5, tgs[idx][i]->GetErrorY(j));
        }
        tgs[idx][i]->SetLineColorAlpha(mcolor[idx], 0.5);
        tgs[idx][i]->SetFillColor(mcolor[idx]);
        tgs[idx][i]->SetFillStyle(4000);
    }
}

void PlotManager::SetNp(int np) {
    this->np = np;
    gcll->SetNp(np);
}

void PlotManager::SetNp(int i, int np) {
    this->np = np;
    gcll->SetNp(i, np);
}

void PlotManager::ClearGcll() {
    gcll->Clear();
}
void PlotManager::SetLegendN(int n) {
    nLeg = n;
}

void PlotManager::SetLegend(int idx, int order, const char* t, const char* s) {
    leg_idx[idx] = order;
    leg_text[idx] = const_cast<char*>(t);
    leg_style[idx] = const_cast<char*>(s);
}

void PlotManager::Plot() {
    c->Clear();
    c->cd();
    gStyle->SetOptStat(0);
    gStyle->SetGridColor(17);

    // C1 and C3
    const char* pdname1 = "c123";
    const int nx = 1;
    const int ny = 2;
    CanvasPartition(c, pdname1, nx, ny, 0.08, 0.745, 0.12, 0.02);
    TPad* pads[4][nx][ny];
    // TPad* pad1[nx][ny];
    for (int i=0; i<nx; i++) {
        for (int j=0; j<ny; j++) {
            char pname1[20];
            sprintf(pname1,"%s_%i_%i",pdname1, i, j);
            pads[0][i][j] = (TPad*) gROOT->FindObject(pname1);
            pads[0][i][j]->SetFillStyle(4000);
            pads[0][i][j]->SetFrameFillStyle(4000);
            pads[0][i][j]->SetTicks(1, 1);
            pads[0][i][j]->SetFillColor(0);
            pads[0][i][j]->SetGrid();
            pads[0][i][j]->Draw();
        }
    }
    // C2 C4
    const char* pdname2 = "c456";
    CanvasPartition(c, pdname2, nx, ny, 0.315, 0.51, 0.12, 0.02);
    // TPad* pad2[nx][ny];
    for (int i=0; i<nx; i++) {
        for (int j=0; j<ny; j++) {
            char pname2[20];
            sprintf(pname2,"%s_%i_%i",pdname2, i, j);
            pads[1][i][j] = (TPad*) gROOT->FindObject(pname2);
            pads[1][i][j]->SetFillStyle(4000);
            pads[1][i][j]->SetFrameFillStyle(4000);
            pads[1][i][j]->SetTicks(1, 1);
            pads[1][i][j]->SetFillColor(0);
            pads[1][i][j]->SetGrid();
            pads[1][i][j]->Draw();
        }
    }
    // R21 R32
    const char* pdname3 = "r123";
    CanvasPartition(c, pdname3, nx, ny, 0.57, 0.255, 0.12, 0.02);
    // TPad* pad3[nx][ny];
    for (int i=0; i<nx; i++) {
        for (int j=0; j<ny; j++) {
            char pname3[20];
            sprintf(pname3,"%s_%i_%i",pdname3, i, j);
            pads[2][i][j] = (TPad*) gROOT->FindObject(pname3);
            pads[2][i][j]->SetFillStyle(4000);
            pads[2][i][j]->SetFrameFillStyle(4000);
            pads[2][i][j]->SetTicks(1, 1);
            pads[2][i][j]->SetFillColor(0);
            pads[2][i][j]->SetGrid();
            pads[2][i][j]->Draw();
        }
    }
    // R42
    const char* pdname4 = "r45";
    CanvasPartition(c, pdname4, nx, ny-1, 0.805, 0.02, 0.55, 0.02);
    // TPad* pad4[nx][ny];
    for (int i=0; i<nx; i++) {
        for (int j=0; j<ny-1; j++) {
            char pname4[20];
            sprintf(pname4,"%s_%i_%i",pdname4, i, j);
            pads[3][i][j] = (TPad*) gROOT->FindObject(pname4);
            pads[3][i][j]->SetFillStyle(4000);
            pads[3][i][j]->SetFrameFillStyle(4000);
            pads[3][i][j]->SetTicks(1, 1);
            pads[3][i][j]->SetFillColor(0);
            pads[3][i][j]->SetGrid();
            pads[3][i][j]->Draw();
        }
    }
    // set tick size
    float ytkSize[7] = {
        0.04, 0.045, 0.025, 0.025, 0.015, 0.02, 0.01
    }; // y tick size
    float xtkSize[7] = {
        0.04, 0.04, 0.04, 0.06, 0.06, 0.08, 0.08
    }; // x tick size

    int subIdx = 0;
    for (int iser=0; iser<4; iser++) { // index of series (columns)
        for (int ipnl=1; ipnl>=0; ipnl--) { // index if pannels
            // skip the last one subplot
            if (iser == 3 && ipnl == 1) {
                continue;
            }
            pads[iser][0][ipnl]->cd();
            th1[subIdx] = (TH1D*)gPad->DrawFrame(x1, 0, x2, 1);
            th1[subIdx]->GetXaxis()->SetNdivisions(505);
            th1[subIdx]->GetYaxis()->SetNdivisions(505);
            th1[subIdx]->GetXaxis()->SetDecimals(kTRUE);
            th1[subIdx]->GetYaxis()->SetDecimals(kTRUE);
            th1[subIdx]->GetXaxis()->SetLabelFont(63);
            th1[subIdx]->GetYaxis()->SetLabelFont(63);
            th1[subIdx]->GetXaxis()->SetLabelSize(19);
            th1[subIdx]->GetYaxis()->SetLabelSize(19);
            th1[subIdx]->GetXaxis()->SetTickSize(xtkSize[subIdx]);
            th1[subIdx]->GetYaxis()->SetTickSize(ytkSize[subIdx]);

            gcll->Clear();
            for (int itf=0; itf<nf; itf++) {
                gcll->Add(tgs[itf][subIdx]);
            }
            gcll->SetFrameAxisRange(th1[subIdx]);

            for (int itf=0; itf<nf; itf++) {
                tgs[itf][subIdx]->Draw(pstyle[itf]);
            }

            // line y = 1 for C4/C2
            if (subIdx == 6) {
                tl->Draw();
            }
            subIdx ++;
        }
    }

    // add labels

    c->cd();
    SetLatexMode(false);

    const char* xtitles[4] = {
        "<N_{part}>", "|y| < y_{max}", "y_{c}", "p_{T} < P_{T}^{max}"
    };

    const char* cNames[2] = { // the name 
        "Cumulants",
        "Factorial Cumulants"
    };

    const char* rNames[2] = { // ratio name
        "Cumulant Ratios",
        "Ratios"
    };

    const double cNameY[2] = {
        0.445, 0.245
    };

    const double rNameY[2] = {
        0.395, 0.425
    };


    const char* labels[2][nc] = {
        {
            "C_{1}", "C_{3}", "C_{2}", "C_{4}",
            "C_{2}/C_{1}", "C_{3}/C_{2}", "C_{4}/C_{2}"
        },
        {
            "#kappa_{1}", "#kappa_{3}", "#kappa_{2}", "#kappa_{4}",
            "#kapps_{2}/#kapps_{1}", "#kapps_{3}/#kapps_{1}", "#kapps_{4}/#kapps_{1}"
        }
    };

    share_lat->DrawLatexNDC(0.140, 0.043, xtitles[sMode]);
    share_lat->DrawLatexNDC(0.365, 0.043, xtitles[sMode]);
    share_lat->DrawLatexNDC(0.625, 0.043, xtitles[sMode]);
    share_lat->DrawLatexNDC(0.860, 0.460, xtitles[sMode]);

    share_lat->SetTextAngle(90);
    share_lat->DrawLatexNDC(0.043, cNameY[cMode], cNames[cMode]);
    share_lat->DrawLatexNDC(0.525, rNameY[cMode], rNames[cMode]);

    share_lat->SetTextAngle(0);
    for (int ic=0; ic<nc; ic++) {
        if (!legacy && pMode == 0 && cMode == 0 && ic == 4) { // netp, R21->R2s
            share_lat->DrawLatexNDC(labelX[ic], labelY[ic], "C_{2}/<p+#bar{p}>");
        } else if (!legacy && cMode == 0 && ic == 4) { // any, R32->R31
            share_lat->DrawLatexNDC(labelX[ic], labelY[ic], "C_{3}/C_{1}");
        } else {
            share_lat->DrawLatexNDC(labelX[ic], labelY[ic], labels[cMode][ic]);
        }
    }

    TLegend* leg = new TLegend(0.785, 0.060, 0.945, 0.280);
    leg->SetLineWidth(0);
    leg->SetFillStyle(0);
    leg->SetTextFont(23);
    for (int ileg=0; ileg<nLeg; ileg++) {
        leg->AddEntry(tgs[leg_idx[ileg]][0], leg_text[ileg], leg_style[ileg]);
    }

    leg->Draw("same");
}

void PlotManager::AddLegend(const char* eng, double y) {
    SetLatexMode(true);
    const char* pNames[3] = {
        "Net-proton", "Proton", "Antiproton"
    };
    share_lat->DrawLatexNDC(0.775, 0.40, Form("STAR Au+Au @ %s GeV", eng));
    std::cout << "sMode:" << sMode << std::endl;
    if (sMode == 0) { // centrality
        share_lat->DrawLatexNDC(0.775, 0.35, Form("%s |y| < %.1f", pNames[pMode], y));
    } else if (sMode == 1) { // trad y scan
        share_lat->DrawLatexNDC(0.775, 0.35, Form("%s |y| < y_{max}", pNames[pMode]));
    } else if (sMode == 2) { // shift y scan
        share_lat->DrawLatexNDC(0.775, 0.35, Form("%s |y-y_{c}| < %.2f", pNames[pMode], y));
    } else if (sMode == 3) { // pt scan
        share_lat->DrawLatexNDC(0.775, 0.35, Form("%s p_{T} < p_{T}^{max}", pNames[pMode]));
    }
    if (sMode != 3) {
        share_lat->DrawLatexNDC(0.775,0.30,"0.4 < p_{T} < 2.0 (GeV/c)");
    } else {
        share_lat->DrawLatexNDC(0.775,0.30,"-0.5 < y < 0.5");
    }
}

void PlotManager::Save(const char* name) {
    c->Print(name);
}
