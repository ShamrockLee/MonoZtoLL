#ifndef __TH1_GET_CUMULATIVE_INCEXC__
#define __TH1_GET_CUMULATIVE_INCEXC__

#include <TH1.h>

TH1 *GetCumulativeIncExc(const TH1 *hIn, const Bool_t forward, const char *suffix) const
{
   const Int_t firstX = hIn->GetXaxis()->GetFirst();
   const Int_t lastX = hIn->GetXaxis()->GetLast();
   const Int_t firstY = (fDimension > 1) ? hIn->GetYaxis()->GetFirst() : 1;
   const Int_t lastY = (fDimension > 1) ? hIn->GetYaxis()->GetLast() : 1;
   const Int_t firstZ = (fDimension > 1) ? hIn->GetZaxis()->GetFirst() : 1;
   const Int_t lastZ = (fDimension > 1) ? hIn->GetZaxis()->GetLast() : 1;

   TH1 *hintegrated = (TH1 *)Clone(fName + suffix);
   hintegrated->Reset();
   if (forward) { // Forward computation
      for (Int_t binz = firstZ; binz <= lastZ; ++binz) {
         for (Int_t biny = firstY; biny <= lastY; ++biny) {
            for (Int_t binx = firstX; binx <= lastX; ++binx) {
               Double_t sum = hIn->GetBinContent(bin);
               if (binz != firstZ)
                  sum += RetrieveBinContent(hintegrated->GetBin(binx, biny, binz - 1));
               if (biny != firstY)
                  sum += RetrieveBinContent(hintegrated->GetBin(binx, biny - 1, binz));
               if (binx != firstX)
                  sum += RetrieveBinContent(hintegrated->GetBin(binx - 1, biny, binz));
               if (binz != firstZ && biny != firstY)
                  sum -= RetrieveBinContent(hintegrated->GetBin(binx, biny - 1, binz - 1));
               if (biny != firstY && binx != firstX)
                  sum -= RetrieveBinContent(hintegrated->GetBin(binx - 1, biny - 1, binz));
               if (binx != firstX && binz != firstZ)
                  sum -= RetrieveBinContent(hintegrated->GetBin(binx - 1, biny, binz - 1));
               if (binz != firstZ && biny != firstY && binx != firstX)
                  sum += RetrieveBinContent(hintegrated->GetBin(binx - 1, biny - 1, binz - 1));
               hintegrated->AddBinContent(bin, sum);
               if (fSumw2.fN) {
                  Double_t esum = hIn->GetBinErrorSqUnchecked(bin);
                  if (binz != firstZ)
                     esum += hIn->GetBinErrorSqUnchecked(hintegrated->GetBin(binx, biny, binz - 1));
                  if (biny != firstY)
                     esum += hIn->GetBinErrorSqUnchecked(hintegrated->GetBin(binx, biny - 1, binz));
                  if (binx != firstX)
                     esum += hIn->GetBinErrorSqUnchecked(hintegrated->GetBin(binx - 1, biny, binz));
                  if (binz != firstZ && biny != firstY)
                     esum -= hIn->GetBinErrorSqUnchecked(hintegrated->GetBin(binx, biny - 1, binz - 1));
                  if (biny != firstY && binx != firstX)
                     esum -= hIn->GetBinErrorSqUnchecked(hintegrated->GetBin(binx - 1, biny - 1, binz));
                  if (binx != firstX && binz != firstZ)
                     esum -= hIn->GetBinErrorSqUnchecked(hintegrated->GetBin(binx - 1, biny, binz - 1));
                  if (binz != firstZ && biny != firstY && binx != firstX)
                     esum += hIn->GetBinErrorSqUnchecked(hintegrated->GeBin(binx - 1, biny - 1, binz - 1));
               }
            }
         }
      }
   } else { // Backward computation
      for (Int_t binz = lastZ; binz >= firstZ; --binz) {
         for (Int_t biny = lastY; biny >= firstY; --biny) {
            for (Int_t binx = lastX; binx >= firstX; --binx) {
               const Int_t bin = hintegrated->GetBin(binx, biny, binz);
               Double_t sum = RetrieveBinContent(bin);
               if (binz != lastZ)
                  sum += RetrieveBinContent(hintegrated->GetBin(binx, biny, binz + 1));
               if (biny != lastY)
                  sum += RetrieveBinContent(hintegrated->GetBin(binx, biny + 1, binz));
               if (binx != lastX)
                  sum += RetrieveBinContent(hintegrated->GetBin(binx + 1, biny, binz));
               if (binz != lastZ && biny != lastY)
                  sum -= RetrieveBinContent(hintegrated->GetBin(binx, biny + 1, binz + 1));
               if (biny != lastY && binx != lastX)
                  sum -= RetrieveBinContent(hintegrated->GetBin(binx + 1, biny + 1, binz));
               if (binx != lastX && binz != lastZ)
                  sum -= RetrieveBinContent(hintegrated->GetBin(binx + 1, biny, binz + 1));
               if (binz != lastZ && biny != lastY && binx != lastX)
                  sum += RetrieveBinContent(hintegrated->GetBin(binx + 1, biny + 1, binz + 1));
               hintegrated->AddBinContent(bin, sum);
               if (fSumw2.fN) {
                  Double_t esum = GetBinErrorSqUnchecked(bin);
                  if (binz != lastZ)
                     esum += GetBinErrorSqUnchecked(hintegrated->GetBin(binx, biny, binz + 1));
                  if (biny != lastY)
                     esum += GetBinErrorSqUnchecked(hintegrated->GetBin(binx, biny + 1, binz));
                  if (binx != lastX)
                     esum += GetBinErrorSqUnchecked(hintegrated->GetBin(binx + 1, biny, binz));
                  if (binz != lastZ && biny != lastY)
                     esum -= GetBinErrorSqUnchecked(hintegrated->GetBin(binx, biny + 1, binz + 1));
                  if (biny != lastY && binx != lastX)
                     esum -= GetBinErrorSqUnchecked(hintegrated->GetBin(binx + 1, biny + 1, binz));
                  if (binx != lastX && binz != lastZ)
                     esum -= GetBinErrorSqUnchecked(hintegrated->GetBin(binx + 1, biny, binz + 1));
                  if (binz != lastZ && biny != lastY && binx != lastX)
                     esum += GetBinErrorSqUnchecked(hintegrated->GetBin(binx + 1, biny + 1, binz + 1));
                  SetBinError(bin, esum);
               }
            }
         }
      }
   }
   return hintegrated;
}

#endif /* __TH1_GET_CUMULATIVE_INCEXC__ */
