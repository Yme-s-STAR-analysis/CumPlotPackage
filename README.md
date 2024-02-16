# Cumulant plots maker

Author: yghuang

Version: 2.0

Date: 16.02.2024

## Quick Start

### Main code

* `nf` is the number of root files you need, in the example, 7 (UrQMD + 2 BES-I + 2 BES-II with RefMult3 + 2 BES-II with RefMult3X) files are used.

* `nc` is the number of cumulants (do not change it).

* `fNames[nf]` should be replaced with the real path to corresponding root files.

* `mcolors`, `mstyle`, `msize` are marker color, style and size. The order is the same as input files.

* `pstyle` means the plot style, its order is the same as input files.

* `sname` is the save name, like `Cent.pdf`

* `sMode` should be 0, 1, 2 or 3. 0 for centrality scan, 1 for rapidity scan and 3 for pT scan. 2 is shifting rapidity scan.

* `pMode` 0, 1, 2 for net-proton, proton and antiproton.

* `cMode` 0 for cumulants and 1 for factorial cumulants. Note that `pMode == 0` and `cMode == 1` make conflict.

* If you want to manually set the location of label (like C4/C2), use `PlotManager::SetLabelLocationX(int, int)`.

* If you have something to plot as a band (like UrQMD), use `PlotManager::SetUrQMD(int)`, this argument is the index of corresponding input file.

* More details see `PlotCent.cxx` as an example.

### Diff code

* Replace the path to root files you want to compare and run the macro.

## Patch Note:

16.02.2024 by yghuang (v2.0):

> Completed version with a nice demo.