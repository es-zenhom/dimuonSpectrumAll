#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "Math/Vector4Dfwd.h"
#include "Math/Vector4D.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TLatex.h"
#include "TStyle.h"


using namespace ROOT::VecOps;

// Compute the invariant mass of all possible dimuon pairs
RVec<float> computeInvariantMass(unsigned int nMuon, RVec<float>& pt, RVec<float>& eta, RVec<float>& phi, RVec<float>& mass, RVec<int>& charge)
{
    // Create vector of all dimuon mass combinations
    RVec<float> dimuon_mass;
    dimuon_mass.reserve((nMuon/2) * ((nMuon + 1)/2));   // maximum possible number of pairs

    //permuting event with more than 2 muons                                    
    for(size_t i = 0; i <= nMuon - 2; i++)
    {
        for(size_t j = i + 1; j <= nMuon - 1; j++)
        {
            // Select events with two muons of opposite charge                  
            if(charge[i] != charge[j])
            {
                ROOT::Math::PtEtaPhiMVector m1(pt[i], eta[i], phi[i], mass[i]);
                ROOT::Math::PtEtaPhiMVector m2(pt[j], eta[j], phi[j], mass[j]);
                // Add element to vector
                dimuon_mass.push_back((m1 + m2).mass());
            }
        }
    }

    // Return all valid combinations
    return dimuon_mass;
}


void dimuonSpectrumAll() {
    // Enable multi-threading
    // The default here is set to a single thread. You can choose the number of threads based on your system.
    ROOT::EnableImplicitMT(8);

    // Create dataframe from NanoAOD files
    //ROOT::RDataFrame df("Events", "root://eospublic.cern.ch//eos/opendata/cms/derived-data/AOD2NanoAODOutreachTool/Run2012BC_DoubleMuParked_Muons.root");
    ROOT::RDataFrame df("Events", "Run2012BC_DoubleMuParked_Muons.root");

    // Select events with two or more muons
    auto df_2mu = df.Filter("nMuon >= 2", "Events with two or more muons");

    // Compute invariant mass of the dimuon system
    auto df_mass = df_2mu.Define("Dimuon_mass", computeInvariantMass,
                                {"nMuon", "Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass", "Muon_charge"});

    // Book histogram of dimuon mass spectrum
    const auto bins = 30000; // Number of bins in the histogram
    const auto low = 0.25; // Lower edge of the histogram
    const auto up = 300.0; // Upper edge of the histogram
    auto hist = df_mass.Histo1D({"", "", bins, low, up}, "Dimuon_mass");

    // Request cut-flow report
    auto report = df_mass.Report();

    // Create canvas for plotting
    gStyle->SetOptStat(0);
    gStyle->SetTextFont(42);
    auto c = new TCanvas("c", "", 800, 700);
    c->SetLogx();
    c->SetLogy();

    // Draw histogram
    hist->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
    hist->GetXaxis()->SetTitleSize(0.04);
    hist->GetYaxis()->SetTitle("N_{Events}");
    hist->GetYaxis()->SetTitleSize(0.04);
    hist->DrawClone();

    // Draw labels
    TLatex label;
    label.SetTextAlign(22);
    label.DrawLatex(0.55, 3.0e4, "#eta");
    label.DrawLatex(0.77, 7.0e4, "#rho,#omega");
    label.DrawLatex(1.20, 4.0e4, "#phi");
    label.DrawLatex(4.40, 1.0e5, "J/#psi");
    label.DrawLatex(4.60, 1.0e4, "#psi'");
    label.DrawLatex(12.0, 2.0e4, "Y(1,2,3S)");
    label.DrawLatex(91.0, 1.5e4, "Z");
    label.SetNDC(true);
    label.SetTextAlign(11);
    label.SetTextSize(0.04);
    label.DrawLatex(0.10, 0.92, "#bf{CMS Open Data}");
    label.SetTextAlign(31);
    label.DrawLatex(0.90, 0.92, "#sqrt{s} = 8 TeV, L_{int} = 11.6 fb^{-1}");

    // Save plot
    c->SaveAs("dimuonSpectrumAll1.pdf");

    // Print cut-flow report
    report->Print();
}


int main() {
    dimuonSpectrumAll();
}
