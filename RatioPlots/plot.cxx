#include "TFile.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TPad.h"
#include "TLegend.h"
#include "TH1.h"
#include "TLatex.h"

#include "../GraphCollection.h"


void plot() {
    const int nf = 2;
    TFile* tf[nf];
    tf[0] = new TFile("/Path-to-BES1/y0p5.root");
    tf[1] = new TFile("/Path-to-BES2/y0p5.root");

    int ms[nf] = {24, 20};
    int mc[nf] = {1, 2};

    TGraphErrors* tgs[nf][6]; // c1 ~ c4
    TGraphErrors* tgdiff[6]; // c1 ~ c4
    double x[9] = {0};
    double xerr[9] = {0};
    double y[9] = {0};
    double yerr[9] = {0};
    double y1, y2, yerr1, yerr2; // 1 and 2 for BES1 or 2
    
    TCanvas* c = new TCanvas();
    c->Print("Diff.pdf(");
    TPad* p1 = new TPad("pad1", "", 0.05, 0.4, 0.95, 0.95); // upper pannel
    TPad* p2 = new TPad("pad2", "", 0.05, 0.05, 0.95, 0.4); // lower pannel
    p1->SetBottomMargin(0);
    p1->SetTopMargin(0);
    p1->SetLeftMargin(0.04);
    p1->SetRightMargin(0);
    p2->SetBottomMargin(0);
    p2->SetTopMargin(0);
    p2->SetLeftMargin(0.04);
    p2->SetRightMargin(0);

    // get grapherrors
    const char* names[6] = {
        "C1", "C2", "C3", "C4", "C5", "C6"
    };
    for (int i=0; i<nf; i++) {
        for (int j=0; j<6; j++) {
            tgs[i][j] = (TGraphErrors*)tf[i]->Get(
                Form("Netp_%s", names[j])
            );
            tgs[i][j]->SetMarkerStyle(ms[i]);
            tgs[i][j]->SetMarkerColor(mc[i]);
            tgs[i][j]->SetLineColor(mc[i]);
            if (i == 0) { // bes1
                tgs[i][j]->SetMarkerSize(1.8);
            } else { // bes2
                tgs[i][j]->SetMarkerSize(1.3);
            }
        }
    }

    for (int j=0; j<6; j++) {
        // get ratio and error
        for (int k=0; k<9; k++) {
            tgs[1][j]->SetPointX(k, tgs[0][j]->GetPointX(k));
            x[k] = tgs[0][j]->GetPointX(k);
            y1 = tgs[0][j]->GetPointY(k);
            y2 = tgs[1][j]->GetPointY(k);
            yerr1 = tgs[0][j]->GetErrorY(k);
            yerr2 = tgs[1][j]->GetErrorY(k);
            y[k] = y2 == 0 ? 0 : y1 / y2;
            yerr[k] = sqrt(pow(y1 == 0 ? 0 : yerr1/y1, 2) + pow(y2 == 0 ? 0 : yerr2/y2, 2)) * (y2 == 0 ? 0 : y1 / y2);
        }
        tgdiff[j] = new TGraphErrors(9, x, y, xerr, yerr);
        tgdiff[j]->SetLineColor(1);
        tgdiff[j]->SetLineStyle(2);
        tgdiff[j]->SetMarkerColor(1);
        tgdiff[j]->SetMarkerStyle(20);
    }

    GraphCollection* gcll = new GraphCollection();
    gcll->SetNp(9);

    TH1* th1s[2][6];
    c->Clear();
    c->cd();
    p1->Draw();
    p1->SetBottomMargin(0);
    p2->Draw();
    p2->SetTopMargin(0);
    p2->SetBottomMargin(0.15);

    TLatex* lat = new TLatex();
    for (int j=0; j<6; j++) {

        p1->Clear();
        p1->cd();
        gcll->Clear();
        gcll->Add(tgs[0][j]);
        gcll->Add(tgs[1][j]);
        th1s[0][j] = (TH1*)p1->DrawFrame(-20, 0, 390, 1);
        th1s[0][j]->GetYaxis()->SetLabelSize(0.04);
        gcll->SetFrameAxisRange(th1s[0][j]);
        tgs[0][j]->Draw("epl");
        tgs[1][j]->Draw("epl");
        lat->SetTextSize(0.1);
        lat->DrawLatexNDC(0.89, 0.14, names[j]);
        if (j == 3) { // draw text and legend only for C4
            // text
            lat->SetTextSize(0.08);
            lat->DrawLatexNDC(0.44, 0.72, "STAR BES-II");
            lat->DrawLatexNDC(0.44, 0.64, "Au + Au @ 14.6 GeV");
            lat->DrawLatexNDC(0.44, 0.56, "Net-proton");
            lat->SetTextSize(0.06);
            lat->DrawLatexNDC(0.44, 0.48, "0.4 < p_{T} < 2.0 GeV/c");
            lat->DrawLatexNDC(0.44, 0.40, "|y| < 0.5");
        }

        p2->Clear();
        p2->cd();
        gcll->Clear();
        gcll->Add(tgdiff[j]);
        th1s[1][j] = (TH1*)p2->DrawFrame(-20, 0, 390, 1);
        // th1s[1][j]->GetXaxis()->SetTitle("<N_{part}>");
        // set tick size 
        th1s[1][j]->GetXaxis()->SetTickSize(0.08);
        th1s[1][j]->GetYaxis()->SetTickSize(0.025);
        th1s[1][j]->GetXaxis()->SetLabelSize(0.06);
        th1s[1][j]->GetYaxis()->SetLabelSize(0.06);
        gcll->SetFrameAxisRange(th1s[1][j]);
        tgdiff[j]->Draw("epl");
        // x title
        lat->SetTextSize(0.06);
        lat->SetTextAlign(22);
        lat->DrawLatexNDC(0.5, 0.04, "<N_{part}>");
        lat->SetTextAlign(11);
        if (j == 3) { // draw text and legend only for C4
            TLegend* leg = new TLegend(0.15, 0.52, 0.45, 0.89);
            leg->SetFillColorAlpha(0, 0);
            leg->SetLineWidth(0);
            leg->AddEntry(tgs[0][0], "BES-I", "p");
            leg->AddEntry(tgs[1][0], "BES-II", "p");
            leg->AddEntry(tgdiff[0], "Black / Red", "p");
            p1->cd();
            leg->Draw("same");
        }

        c->Print("Diff.pdf");
    }
    c->Clear();
    c->Print("Diff.pdf)");

    tf[0]->Close();
    tf[1]->Close();
}
