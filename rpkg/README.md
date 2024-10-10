# Open Redatam (R Package) <img src="man/figures/logo.svg" align="right" height="139" alt="" />

[![Ubuntu app](https://github.com/pachadotdev/open-redatam/actions/workflows/build-ubuntu.yml/badge.svg)](https://github.com/pachadotdev/open-redatam/actions/workflows/build-ubuntu.yml)
[![Mac app](https://github.com/pachadotdev/open-redatam/actions/workflows/build-mac.yml/badge.svg)](https://github.com/pachadotdev/open-redatam/actions/workflows/build-mac.yml)
[![Build Windows executables](https://github.com/pachadotdev/open-redatam/actions/workflows/build-windows.yml/badge.svg)](https://github.com/pachadotdev/open-redatam/actions/workflows/build-windows.yml)
[![R Package](https://github.com/pachadotdev/open-redatam/actions/workflows/build-rpkg.yml/badge.svg)](https://github.com/pachadotdev/open-redatam/actions/workflows/build-rpkg.yml)
[![BuyMeACoffee](https://raw.githubusercontent.com/pachadotdev/buymeacoffee-badges/main/bmc-donate-white.svg)](https://buymeacoffee.com/pacha)

## About

Open Redatam is a software for extracting raw information from REDATAM databases.

For the standalone C++ command line application and desktop app, see the [main directory](https://github.com/pachadotdev/open-redatam/) of this repository.

To install the R package, run the following command:

```r
remotes::install_github("pachadotdev/open-redatam/rpkg", subdir = "rpkg")
```

## Usage

For a given census, such as the [Chilean Census 2017](https://redatam.org/cdr/descargas/censos/poblacion/CP2017CHL.zip), run the following command:

```r
redatam::read_redatam("input-dir/dictionary.dicx")
```

Please read the vignette for a more detailed explanation and how this package can be used in conjunction with `dplyr` and other packages.

## Differences with the C++ standalone application

The R package uses a modified copy of the C++ code to read the REDATAM databases that parses data into a list of tibbles instead of writing to CSV files.

## Credits

Open Redatam was created and is supported by Lital Barkai (barkailital@gmail.com).

The tests, installation instructions and R package were created by Mauricio "Pacha" Vargas Sepulveda (m.sepulveda@mail.utoronto.ca)

The original converter was created by [Pablo De Grande](https://github.com/discontinuos). See [here](https://www.scielo.org.mx/scielo.php?script=sci_arttext&pid=S0186-72102016000300811) for more information.

This project uses [pugixml](https://github.com/zeux/pugixml) created by Arseny Kapoulkine to structure a part of the output data.
