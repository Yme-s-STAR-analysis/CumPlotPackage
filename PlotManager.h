#ifndef __PMHEADER__
#define __PMHEADER__
#define FILEMAX 10

using pMode_t = int;
using cMode_t = int;
using sMode_t = int;

class TFile;
class TGraphErrors;
class GraphCollection;
class TLine;
class TLatex;
class TH1D;
class TCanvas;

class PlotManager {

    private:
        static const int nc = 7; // for maximum 4th order 
        // static const int nc = 11; // for maximum 6th order 
        int nf;
        int mcolor[FILEMAX];
        int mstyle[FILEMAX];
        double msize[FILEMAX];
        int np;
        char* pName;
        char* pstyle[FILEMAX];
        TFile* tfs[FILEMAX];
        TGraphErrors* tgs[FILEMAX][nc];
        TCanvas* c;
        GraphCollection* gcll;
        double x1, x2; // X axis
        TLine* tl;
        TH1D* th1[nc];

        int cMode; // cumulants or c.f.
        int pMode; // netp, pro, pbar
        int sMode; // centrality, y scan
        double labelX[nc]; // the label like C_{1} location (NDC)
        double labelY[nc];
        TLatex* share_lat;

        int nLeg;
        int leg_idx[FILEMAX];
        char* leg_text[FILEMAX];
        char* leg_style[FILEMAX];

        bool legacy;

    public:
        PlotManager();
        ~PlotManager() {}
        void SetLegacy(bool val) { legacy = val; }
        void Init(int, const char**, int*, int*, double*, const char**, pMode_t, cMode_t, sMode_t);
        void Init(int, const char**, int*, int*, double*, const char**, pMode_t, cMode_t, sMode_t, bool);
        void SetLatexMode(bool);
        void SetDefaultLabelLocation();
        void SetLabelLocationX(int, double);
        void SetLabelLocationY(int, double);
        void SetLabelLocation(int, double, double);
        void ReadGraph();
        void SetUrQMD(int);
        void SetNp(int);
        void SetNp(int, int);
        void ClearGcll();
        void SetGraphXArray(int, int, double*);
        void SetLegendN(int);
        void SetLegend(int, int, const char*, const char*);
        void Plot();
        void AddLegend(const char*, double);
        void Save(const char*);

};


#endif