class GraphCollection {
    private:
        static const int nf = 20;
        TGraphErrors* tgs[nf];
        int n;
        int np[nf];
        double y1, y2;

    public:
        GraphCollection() {
            Clear();
            for (int i=0; i<nf; i++) { np[i] = 9; }
        }
        ~GraphCollection() {}

        void Clear() {
            n = 0;
            y1 = 9999.0;
            y2 = -9999.0;
            for (int i=0; i<nf; i++) {
                tgs[i] = 0;
            }
        }

        void Add(TGraphErrors* tg) {
            if (n >= nf) { return; }
            tgs[n] = tg;
            n += 1;
        }

        void SetNp(int value) { 
            for (int i=0; i<nf; i++) {
                np[i] = value; 
            }
        }
        void SetNp(int i, int value) { np[i] = value; }

        void FindProperAxisRange(double fac = 1.6) {
            double tmp1 = 0.0;
            double tmp2 = 0.0;
            for (int i=0; i<n; i++) {
                for (int j=0; j<np[i]; j++) {
                    int tmpn = tgs[i]->GetN();
                    if (j >= tmpn) { continue; }
                    tmp1 = tgs[i]->GetPointY(j) + tgs[i]->GetErrorY(j);
                    tmp2 = tgs[i]->GetPointY(j) - tgs[i]->GetErrorY(j);
                    if (tmp2 < y1) { y1 = tmp2; }
                    if (tmp1 > y2) { y2 = tmp1; }
                }
            }

            double halfHeight = (y2 - y1) * 0.5;
            double mean = (y1 + y2) * 0.5;
            y1 = mean - halfHeight * fac;
            y2 = mean + halfHeight * fac;

        }

    double GetY1() { return y1; }
    double GetY2() { return y2; }

    void SetFrameAxisRange(TH1* frame) {
        // set y1 and y2 with proper range
        FindProperAxisRange();
        frame->GetYaxis()->SetRangeUser(y1, y2);
    }
    void SetFrameAxisRange(TH1* frame, double y1, double y2) {
        // directly set y1 and y2
        frame->GetYaxis()->SetRangeUser(y1, y2);
    }

};