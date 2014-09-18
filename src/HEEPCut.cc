#include "UserCode/IIHETree/interface/HEEPCut.h"

#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

//////////////////////////////////////////////////////////////////////////////////////////
//                                      Base class                                      //
//////////////////////////////////////////////////////////////////////////////////////////
HEEPCutBase::HEEPCutBase(std::string name, IIHEModuleHEEP* mod){
  name_ = name ;
  index_ = -1 ;
  nPass_ = 0 ;
  nPassCumulative_ = 0 ;
  reset() ;
  parent_mod_ = mod ;
  branchName_n_= name_ + "_n" ;
  branchName_nCumulative_ = name_ + "_nCumulative" ;
}
HEEPCutBase::~HEEPCutBase(){}
void HEEPCutBase::setStatus(bool value, bool cumulativeSuccess){
  status_ = value ;
  if(status_) nPass_++ ;
  if(status_ && cumulativeSuccess) nPassCumulative_++ ;
}
bool HEEPCutBase::getStatus(){ return status_ ; }
void HEEPCutBase::reset(){ status_ = true ; }
bool HEEPCutBase::addBranches(){
  bool success = true ;
  success = (success && parent_mod_->addBranch(name_                  , kVectorBool)) ;
  success = (success && parent_mod_->addBranch(branchName_n_          , kInt       )) ;
  success = (success && parent_mod_->addBranch(branchName_nCumulative_, kInt       )) ;
  return success ;
}
void HEEPCutBase::store(){ parent_mod_->store(name_, status_) ; }
bool HEEPCutBase::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){ return status_ ; }
void HEEPCutBase::beginEvent(){
  nPass_           = 0 ;
  nPassCumulative_ = 0 ;
}
void HEEPCutBase::endEvent(){
  parent_mod_->store(branchName_n_          , nPass_          ) ;
  parent_mod_->store(branchName_nCumulative_, nPassCumulative_) ;
}

bool HEEPCutBase::isBarrel(reco::GsfElectronCollection::const_iterator gsfiter){
  return (fabs(gsfiter->superCluster()->eta()) < 1.442) ;
}
bool HEEPCutBase::isEndcap(reco::GsfElectronCollection::const_iterator gsfiter){
  float eta = gsfiter->superCluster()->eta() ;
  return (fabs(eta) > 1.56 && fabs(eta) < 2.5) ;
}
int  HEEPCutBase::detectorRegion(reco::GsfElectronCollection::const_iterator gsfiter){
  if(isBarrel(gsfiter)) return kBarrel ;
  if(isEndcap(gsfiter)) return kEndcap ;
  return kNone ;
}
bool HEEPCutBase::isBarrel_50(reco::GsfElectronCollection::const_iterator gsfiter){
  return (fabs(gsfiter->superCluster()->eta()) < 1.4442) ;
}
bool HEEPCutBase::isEndcap_50(reco::GsfElectronCollection::const_iterator gsfiter){
  float eta = gsfiter->superCluster()->eta() ;
  return (fabs(eta) > 1.566 && fabs(eta) < 2.5) ;
}
int  HEEPCutBase::detectorRegion_50(reco::GsfElectronCollection::const_iterator gsfiter){
  if(isBarrel_50(gsfiter)) return kBarrel ;
  if(isEndcap_50(gsfiter)) return kEndcap ;
  return kNone ;
}

//////////////////////////////////////////////////////////////////////////////////////////
//                                     HEEP 4.1 cuts                                    //
//////////////////////////////////////////////////////////////////////////////////////////
HEEPCut_41_Et              ::HEEPCut_41_Et             (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_41_eta             ::HEEPCut_41_eta            (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_41_EcalDriven      ::HEEPCut_41_EcalDriven     (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_41_dEtaIn          ::HEEPCut_41_dEtaIn         (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_41_dPhiIn          ::HEEPCut_41_dPhiIn         (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_41_HOverE          ::HEEPCut_41_HOverE         (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_41_SigmaIetaIeta   ::HEEPCut_41_SigmaIetaIeta  (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_41_E2x5OverE5x5    ::HEEPCut_41_E2x5OverE5x5   (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_41_isolEMHadDepth1 ::HEEPCut_41_isolEMHadDepth1(std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_41_IsolPtTrks      ::HEEPCut_41_IsolPtTrks     (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_41_missingHits     ::HEEPCut_41_missingHits    (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_41_dxyFirstPV      ::HEEPCut_41_dxyFirstPV     (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;

bool HEEPCut_41_Et             ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  float Et  = gsfiter->caloEnergy()*sin(gsfiter->p4().theta()) ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (Et > 35.0) ; break ;
    case kEndcap: result = (Et > 35.0) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_41_eta            ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  int region = detectorRegion(gsfiter) ;
  bool result = (region==kBarrel || region==kEndcap) ;
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_41_EcalDriven     ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  bool result = gsfiter->ecalDrivenSeed() ;
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_41_dEtaIn         ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  float deltaEta = gsfiter->deltaEtaSuperClusterTrackAtVtx() ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (fabs(deltaEta) < 0.005) ; break ;
    case kEndcap: result = (fabs(deltaEta) < 0.007) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_41_dPhiIn         ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  float deltaPhi = gsfiter->deltaPhiSuperClusterTrackAtVtx() ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (fabs(deltaPhi) < 0.06) ; break ;
    case kEndcap: result = (fabs(deltaPhi) < 0.06) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_41_HOverE         ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  float HOverE = gsfiter->hadronicOverEm() ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (HOverE < 0.05) ; break ;
    case kEndcap: result = (HOverE < 0.05) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_41_SigmaIetaIeta  ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = true ; break ;
    case kEndcap: result = (gsfiter->sigmaIetaIeta() < 0.03) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_41_E2x5OverE5x5   ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (gsfiter->scE2x5Max()/gsfiter->scE5x5() > 0.94) || (gsfiter->scE1x5()/gsfiter->scE5x5() > 0.83) ; break ;
    case kEndcap: result = true ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}

bool HEEPCut_41_isolEMHadDepth1::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  float gsf_ecaliso  = gsfiter->dr03EcalRecHitSumEt() ;
  float gsf_hcaliso1 = gsfiter->dr03HcalDepth1TowerSumEt() ;
  float gsf_gsfet   = gsfiter->caloEnergy()*sin(gsfiter->p4().theta()) ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel:
      result = (gsf_ecaliso+gsf_hcaliso1) < (2.+0.03*gsf_gsfet + rho_*EcalHcal1EffAreaBarrel_) ;
      break ;
    case kEndcap:
      if(gsf_gsfet<50.0){ result = (gsf_ecaliso+gsf_hcaliso1) <  2.5                       + rho_*EcalHcal1EffAreaEndcaps_   ; }
      else              { result = (gsf_ecaliso+gsf_hcaliso1) < (2.5+0.03*(gsf_gsfet-50.0) + rho_*EcalHcal1EffAreaEndcaps_ ) ; }
      break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
void HEEPCut_41_isolEMHadDepth1::setRho(float rho){ rho_ = rho ; }
void HEEPCut_41_isolEMHadDepth1::setEcalHcal1EffAreaBarrel (float EcalHcal1EffAreaBarrel ){ EcalHcal1EffAreaBarrel_  = EcalHcal1EffAreaBarrel  ; }
void HEEPCut_41_isolEMHadDepth1::setEcalHcal1EffAreaEndcaps(float EcalHcal1EffAreaEndcaps){ EcalHcal1EffAreaEndcaps_ = EcalHcal1EffAreaEndcaps ; }

bool HEEPCut_41_IsolPtTrks     ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  float gsf_trackIso = gsfiter->dr03TkSumPt() ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (gsf_trackIso < 5.0) ; break ;
    case kEndcap: result = (gsf_trackIso < 5.0) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_41_missingHits    ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  bool result = (gsfiter->gsfTrack()->trackerExpectedHitsInner().numberOfLostHits() <= 1) ;
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}

bool HEEPCut_41_dxyFirstPV     ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  float gsf_dxy_firstPVtx = gsfiter->gsfTrack()->dxy(firstPV_) ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (fabs(gsf_dxy_firstPVtx) < 0.02) ; break ;
    case kEndcap: result = (fabs(gsf_dxy_firstPVtx) < 0.05) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
void HEEPCut_41_dxyFirstPV     ::setFirstPV(math::XYZPoint PV){ firstPV_ = math::XYZPoint(PV) ; }

//////////////////////////////////////////////////////////////////////////////////////////
//                                   HEEP 5.0 25ns cuts                                 //
//////////////////////////////////////////////////////////////////////////////////////////
HEEPCut_50_25ns_Et              ::HEEPCut_50_25ns_Et             (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_50_25ns_eta             ::HEEPCut_50_25ns_eta            (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_50_25ns_dEtaIn          ::HEEPCut_50_25ns_dEtaIn         (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_50_25ns_dPhiIn          ::HEEPCut_50_25ns_dPhiIn         (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_50_25ns_HOverE          ::HEEPCut_50_25ns_HOverE         (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_50_25ns_SigmaIetaIeta   ::HEEPCut_50_25ns_SigmaIetaIeta  (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_50_25ns_E2x5OverE5x5    ::HEEPCut_50_25ns_E2x5OverE5x5   (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_50_25ns_isolEMHadDepth1 ::HEEPCut_50_25ns_isolEMHadDepth1(std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_50_25ns_IsolPtTrks      ::HEEPCut_50_25ns_IsolPtTrks     (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_50_25ns_EcalDriven      ::HEEPCut_50_25ns_EcalDriven     (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_50_25ns_missingHits     ::HEEPCut_50_25ns_missingHits    (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_50_25ns_dxyFirstPV      ::HEEPCut_50_25ns_dxyFirstPV     (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;

bool HEEPCut_50_25ns_Et             ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  float Et  = gsfiter->caloEnergy()*sin(gsfiter->p4().theta()) ;
  int region = detectorRegion_50(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (Et > 35.0) ; break ;
    case kEndcap: result = (Et > 35.0) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_50_25ns_eta            ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  int region = detectorRegion_50(gsfiter) ;
  bool result = (region==kBarrel || region==kEndcap) ;
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_50_25ns_EcalDriven     ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  bool result = gsfiter->ecalDrivenSeed() ;
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_50_25ns_dEtaIn         ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  float deltaEta = gsfiter->deltaEtaSuperClusterTrackAtVtx() ;
  int region = detectorRegion_50(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel:{
      float Et  = gsfiter->caloEnergy()*sin(gsfiter->p4().theta()) ;
      float threshold = std::max(0.016-1.0e-4*Et, 0.004) ;
      result = (fabs(deltaEta) < threshold) ;
      break ;
    }
    case kEndcap:{
      float Et  = gsfiter->caloEnergy()*sin(gsfiter->p4().theta()) ;
      float threshold = std::max(0.015-8.5e-5*Et, 0.006) ;
      result = (fabs(deltaEta) < threshold) ;
      break ;
    }
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_50_25ns_dPhiIn         ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  float deltaPhi = gsfiter->deltaPhiSuperClusterTrackAtVtx() ;
  int region = detectorRegion_50(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (fabs(deltaPhi) < 0.06) ; break ;
    case kEndcap: result = (fabs(deltaPhi) < 0.06) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_50_25ns_HOverE         ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  float HOverE = gsfiter->hadronicOverEm() ;
  float E      = gsfiter->caloEnergy() ;
  int region = detectorRegion_50(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (HOverE <  2.0/E + 0.05) ; break ;
    case kEndcap: result = (HOverE < 12.5/E + 0.05) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_50_25ns_SigmaIetaIeta  ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  int region = detectorRegion_50(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = true ; break ;
    case kEndcap: result = (gsfiter->sigmaIetaIeta() < 0.03) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_50_25ns_E2x5OverE5x5   ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  int region = detectorRegion_50(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (gsfiter->scE2x5Max()/gsfiter->scE5x5() > 0.94) || (gsfiter->scE1x5()/gsfiter->scE5x5() > 0.83) ; break ;
    case kEndcap: result = true ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}

bool HEEPCut_50_25ns_isolEMHadDepth1::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  float gsf_ecaliso  = gsfiter->dr03EcalRecHitSumEt() ;
  float gsf_hcaliso1 = gsfiter->dr03HcalDepth1TowerSumEt() ;
  float gsf_gsfet    = gsfiter->caloEnergy()*sin(gsfiter->p4().theta()) ;
  int region = detectorRegion_50(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel:
      result = (gsf_ecaliso+gsf_hcaliso1) < (2.+0.03*gsf_gsfet + rho_*EcalHcal1EffAreaBarrel_) ;
      break ;
    case kEndcap:
      if(gsf_gsfet<50.0){ result = (gsf_ecaliso+gsf_hcaliso1) <  2.5                       + rho_*EcalHcal1EffAreaEndcaps_   ; }
      else              { result = (gsf_ecaliso+gsf_hcaliso1) < (2.5+0.03*(gsf_gsfet-50.0) + rho_*EcalHcal1EffAreaEndcaps_ ) ; }
      break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
void HEEPCut_50_25ns_isolEMHadDepth1::setRho(float rho){ rho_ = rho ; }
void HEEPCut_50_25ns_isolEMHadDepth1::setEcalHcal1EffAreaBarrel (float EcalHcal1EffAreaBarrel ){ EcalHcal1EffAreaBarrel_  = EcalHcal1EffAreaBarrel  ; }
void HEEPCut_50_25ns_isolEMHadDepth1::setEcalHcal1EffAreaEndcaps(float EcalHcal1EffAreaEndcaps){ EcalHcal1EffAreaEndcaps_ = EcalHcal1EffAreaEndcaps ; }

bool HEEPCut_50_25ns_IsolPtTrks     ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  float gsf_trackIso = gsfiter->dr03TkSumPt() ;
  int region = detectorRegion_50(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (gsf_trackIso < 5.0) ; break ;
    case kEndcap: result = (gsf_trackIso < 5.0) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_50_25ns_missingHits    ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  bool result = (gsfiter->gsfTrack()->trackerExpectedHitsInner().numberOfLostHits() <= 1) ;
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_50_25ns_dxyFirstPV     ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  float gsf_dxy_firstPVtx = gsfiter->gsfTrack()->dxy(firstPV_) ;
  int region = detectorRegion_50(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (fabs(gsf_dxy_firstPVtx) < 0.02) ; break ;
    case kEndcap: result = (fabs(gsf_dxy_firstPVtx) < 0.05) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
void HEEPCut_50_25ns_dxyFirstPV     ::setFirstPV(math::XYZPoint PV){ firstPV_ = math::XYZPoint(PV) ; }

//////////////////////////////////////////////////////////////////////////////////////////
//                                 HEEP 5.0  startup cuts                               //
//////////////////////////////////////////////////////////////////////////////////////////
HEEPCut_50_50ns_Et              ::HEEPCut_50_50ns_Et             (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_50_50ns_eta             ::HEEPCut_50_50ns_eta            (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_50_50ns_dEtaIn          ::HEEPCut_50_50ns_dEtaIn         (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_50_50ns_dPhiIn          ::HEEPCut_50_50ns_dPhiIn         (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_50_50ns_HOverE          ::HEEPCut_50_50ns_HOverE         (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_50_50ns_SigmaIetaIeta   ::HEEPCut_50_50ns_SigmaIetaIeta  (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_50_50ns_E2x5OverE5x5    ::HEEPCut_50_50ns_E2x5OverE5x5   (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_50_50ns_isolEMHadDepth1 ::HEEPCut_50_50ns_isolEMHadDepth1(std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_50_50ns_IsolPtTrks      ::HEEPCut_50_50ns_IsolPtTrks     (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_50_50ns_EcalDriven      ::HEEPCut_50_50ns_EcalDriven     (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_50_50ns_missingHits     ::HEEPCut_50_50ns_missingHits    (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;
HEEPCut_50_50ns_dxyFirstPV      ::HEEPCut_50_50ns_dxyFirstPV     (std::string name, IIHEModuleHEEP* mod): HEEPCutBase(name, mod){} ;

bool HEEPCut_50_50ns_Et             ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  float Et  = gsfiter->caloEnergy()*sin(gsfiter->p4().theta()) ;
  int region = detectorRegion_50(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (Et > 35.0) ; break ;
    case kEndcap: result = (Et > 35.0) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_50_50ns_eta            ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  int region = detectorRegion_50(gsfiter) ;
  bool result = (region==kBarrel || region==kEndcap) ;
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_50_50ns_EcalDriven     ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  bool result = gsfiter->ecalDrivenSeed() ;
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_50_50ns_dEtaIn         ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  float deltaEta = gsfiter->deltaEtaSuperClusterTrackAtVtx() ;
  int region = detectorRegion_50(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel:{
      float Et  = gsfiter->caloEnergy()*sin(gsfiter->p4().theta()) ;
      float threshold = std::max(0.016-1.0e-4*Et, 0.004) ;
      result = (fabs(deltaEta) < threshold) ;
      break ;
    }
    case kEndcap:{
      result = (fabs(deltaEta) < 0.02) ;
      break ;
    }
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_50_50ns_dPhiIn         ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  float deltaPhi = gsfiter->deltaPhiSuperClusterTrackAtVtx() ;
  int region = detectorRegion_50(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (fabs(deltaPhi) < 0.06) ; break ;
    case kEndcap: result = (fabs(deltaPhi) < 0.15) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_50_50ns_HOverE         ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  float HOverE = gsfiter->hadronicOverEm() ;
  float E      = gsfiter->caloEnergy() ;
  int region = detectorRegion_50(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (HOverE <  2.0/E + 0.05) ; break ;
    case kEndcap: result = (HOverE < 12.5/E + 0.05) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_50_50ns_SigmaIetaIeta  ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  int region = detectorRegion_50(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = true ; break ;
    case kEndcap: result = (gsfiter->sigmaIetaIeta() < 0.03) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_50_50ns_E2x5OverE5x5   ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  int region = detectorRegion_50(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (gsfiter->scE2x5Max()/gsfiter->scE5x5() > 0.94) || (gsfiter->scE1x5()/gsfiter->scE5x5() > 0.83) ; break ;
    case kEndcap: result = true ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}

bool HEEPCut_50_50ns_isolEMHadDepth1::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  float gsf_ecaliso  = gsfiter->dr03EcalRecHitSumEt() ;
  float gsf_hcaliso1 = gsfiter->dr03HcalDepth1TowerSumEt() ;
  float gsf_gsfet   = gsfiter->caloEnergy()*sin(gsfiter->p4().theta()) ;
  int region = detectorRegion_50(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel:
      result = (gsf_ecaliso+gsf_hcaliso1) < (2.0+0.03*gsf_gsfet + rho_*EcalHcal1EffAreaBarrel_) ;
      break ;
    case kEndcap:
      if(gsf_gsfet<50.0){ result = (gsf_ecaliso+gsf_hcaliso1) <  2.5                       + rho_*EcalHcal1EffAreaEndcaps_   ; }
      else              { result = (gsf_ecaliso+gsf_hcaliso1) < (2.5+0.03*(gsf_gsfet-50.0) + rho_*EcalHcal1EffAreaEndcaps_ ) ; }
      break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
void HEEPCut_50_50ns_isolEMHadDepth1::setRho(float rho){ rho_ = rho ; }
void HEEPCut_50_50ns_isolEMHadDepth1::setEcalHcal1EffAreaBarrel (float EcalHcal1EffAreaBarrel ){ EcalHcal1EffAreaBarrel_  = EcalHcal1EffAreaBarrel  ; }
void HEEPCut_50_50ns_isolEMHadDepth1::setEcalHcal1EffAreaEndcaps(float EcalHcal1EffAreaEndcaps){ EcalHcal1EffAreaEndcaps_ = EcalHcal1EffAreaEndcaps ; }

bool HEEPCut_50_50ns_IsolPtTrks     ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  float gsf_trackIso = gsfiter->dr03TkSumPt() ;
  int region = detectorRegion_50(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (gsf_trackIso < 5.0) ; break ;
    case kEndcap: result = (gsf_trackIso < 5.0) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_50_50ns_missingHits    ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  bool result = (gsfiter->gsfTrack()->trackerExpectedHitsInner().numberOfLostHits() <= 1) ;
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
bool HEEPCut_50_50ns_dxyFirstPV     ::applyCut(reco::GsfElectronCollection::const_iterator gsfiter, bool cumulativeSuccess){
  float gsf_dxy_firstPVtx = gsfiter->gsfTrack()->dxy(firstPV_) ;
  int region = detectorRegion_50(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (fabs(gsf_dxy_firstPVtx) < 0.02) ; break ;
    case kEndcap: result = (fabs(gsf_dxy_firstPVtx) < 0.05) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
void HEEPCut_50_50ns_dxyFirstPV     ::setFirstPV(math::XYZPoint PV){ firstPV_ = math::XYZPoint(PV) ; }

//////////////////////////////////////////////////////////////////////////////////////////
//                                   Cut collections                                    //
//////////////////////////////////////////////////////////////////////////////////////////
HEEPCutCollection::HEEPCutCollection(std::string name, IIHEModuleHEEP* mod){
  name_         = name ;
  branchName_n_ = name + "_n" ;
  parent_mod_   = mod ;
  nPass_        = -1 ;
}
HEEPCutCollection::~HEEPCutCollection(){};
void HEEPCutCollection::config(){
  cutIndex_        = 0 ;
  collectionIndex_ = 0 ;
  for(unsigned int i=0 ; i<cutTypes_.size() ; ++i){
    switch(cutTypes_.at(i)){
      case kCut:{
        listOfCuts_.at(cutIndex_)->addBranches() ;
        ++cutIndex_ ;
        break ;
      }
      case kCollection:{
        listOfCutCollections_.at(collectionIndex_)->config() ;
        ++collectionIndex_ ;
        break ;
      }
    }
  }
  parent_mod_->addBranch(name_, kVectorBool) ;
  parent_mod_->addBranch(branchName_n_, kInt) ;
}

void HEEPCutCollection::beginEvent(){
  nPass_ = 0 ;
  cutIndex_        = 0 ;
  collectionIndex_ = 0 ;
  for(unsigned int i=0 ; i<cutTypes_.size() ; ++i){
    switch(cutTypes_.at(i)){
      case kCut:{
        listOfCuts_.at(cutIndex_)->beginEvent() ;
        ++cutIndex_ ;
        break ;
      }
      case kCollection:{
        listOfCutCollections_.at(collectionIndex_)->beginEvent() ;
        ++collectionIndex_ ;
        break ;
      }
    }
  }
}
void HEEPCutCollection::endEvent()  {
  parent_mod_->store(branchName_n_, nPass_) ;
  cutIndex_        = 0 ;
  collectionIndex_ = 0 ;
  for(unsigned int i=0 ; i<cutTypes_.size() ; ++i){
    switch(cutTypes_.at(i)){
      case kCut:{
        listOfCuts_.at(cutIndex_)->endEvent() ;
        ++cutIndex_ ;
        break ;
      }
      case kCollection:{
        listOfCutCollections_.at(collectionIndex_)->endEvent() ;
        ++collectionIndex_ ;
        break ;
      }
    }
  }
}

void HEEPCutCollection::addCut(HEEPCutBase* cut){
  listOfCuts_.push_back(cut) ;
  cutTypes_.push_back(kCut) ;
}
void HEEPCutCollection::addCutCollection(HEEPCutCollection* collection){
  listOfCutCollections_.push_back(collection) ;
  cutTypes_.push_back(kCollection) ;
}
bool HEEPCutCollection::applyCuts(reco::GsfElectronCollection::const_iterator gsfiter){
  return applyCuts(gsfiter, true) ;
}
bool HEEPCutCollection::applyCuts(reco::GsfElectronCollection::const_iterator gsfiter, bool status_in){
  cutIndex_        = 0 ;
  collectionIndex_ = 0 ;
  status_ = status_in ;
  for(unsigned int i=0 ; i<cutTypes_.size() ; ++i){
    switch(cutTypes_.at(i)){
      case kCut:{
        HEEPCutBase* cut = (HEEPCutBase*) listOfCuts_.at(cutIndex_) ;
        cut->applyCut(gsfiter, status_) ;
        cut->store() ;
        ++cutIndex_ ;
        status_ = (status_ && cut->getStatus()) ;
        break ;
      }
      case kCollection:{
        HEEPCutCollection* collection = listOfCutCollections_.at(collectionIndex_) ;
        collection->applyCuts(gsfiter, status_) ;
        ++collectionIndex_ ;
        status_ = (status_ && collection->getStatus()) ;
        break ;
      }
    }
  }
  parent_mod_->store(name_, status_) ;
  if(status_) nPass_++ ;
  return status_ ;
}


