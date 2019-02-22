#include "TString.h"
#include "TMath.h"
#include "AliForwardSettings.h"
#include "TFile.h"

//________________________________________________________________________
AliForwardSettings::AliForwardSettings() :
  fDataType(-1),
  fPhiAcceptanceLowEdge(0),
  fPhiAcceptanceUpEdge(2*TMath::Pi()),
  fEtaLowEdge(-4.0),
  fEtaUpEdge(6.0),
  fNPhiBins(20),
  fZVtxAcceptanceLowEdge(-10),
  fZVtxAcceptanceUpEdge(10),
  fNZvtxBins(10),
  qctype("std"),
  fnoSamples(10),
  fNRefEtaBins(1),
  fNDiffEtaBins(50),
  fCentBins(10),
  nuacentral(),
  nuaforward(),
  seccorr(),
  doNUA(false),
  gap(0.0),
  minpt(0.2),
  maxpt(5),
  mc(kFALSE),
  esd(kFALSE),
  tracktype(kHybrid),
  nua_mode(kNormal),
  useTPC{kTRUE},
  useSPD(kFALSE),
  use_primaries(kFALSE),
  use_primaries_cen(kFALSE),
  use_primaries_fwd(kFALSE),
  centrality_estimator('SPDTracklets'),//CL0, V0M
  etagap(kTRUE),
  makeFakeHoles(kFALSE),
  fnoClusters(70),
  fCutChargedDCAxyMax(0.),
  fCutChargedDCAzMax(0.)
{
}
