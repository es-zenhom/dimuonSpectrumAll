# Dimuon Spectrum Analysis

This repository contains the C++ code used for the Dimuon Spectrum Analysis. The code uses the ROOT library, an open-source data analysis framework developed by CERN. It calculates the invariant mass of all possible dimuon pairs from the provided dataset, and then plots the distribution of these masses, known as the dimuon spectrum.

## File Structure
- `dimuonSpectrum.cpp` : Main C++ script that performs the analysis and produces the histogram.

## Dependencies
This project depends on the following libraries:

1. **ROOT** : For data analysis and visualization. You can install ROOT following the instructions [here](https://root.cern/install/).

## Dataset
The dataset used in this project (`Run2012BC_DoubleMuParked_Muons.root`) comes from the CMS Open Data portal and is not included in the repository due to its size. You can download it from [this link](http://opendata.cern.ch/record/545).

## Usage

To run the program, follow these steps:

1. Clone the repository: `git clone https://github.com/your-username/dimuon-spectrum.git`
2. Move into the directory: `cd dimuon-spectrum`
3. Compile the program: `g++ dimuonSpectrum.cpp $(root-config --cflags --libs) -o dimuonSpectrum`
4. Run the executable: `./dimuonSpectrum`

## Code Overview

- The main function calls `dimuonSpectrumAll()`, which contains the analysis.
- It begins by enabling multithreading for performance and loads the dataset.
- It then filters the dataset to only keep events with two or more muons.
- The `computeInvariantMass` function is used to compute the invariant mass of all possible dimuon pairs. 
- A histogram is then created using this data.
- The histogram is displayed and saved in the form of a PDF file.

**Note:** The code is designed to work with datasets from the CMS detector, but with a bit of adaptation, it could work with data from other particle detectors as well.
