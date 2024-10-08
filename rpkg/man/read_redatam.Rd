% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/read_redatam.R
\name{read_redatam}
\alias{read_redatam}
\title{Read REDATAM dictionary}
\usage{
read_redatam(dictionary)
}
\arguments{
\item{dictionary}{A character string with the path to the dictionary file. It
allows relative paths.}
}
\value{
A list of tibbles, each one representing a table with the census
 entities (or levels) and their attributes (or variables).
}
\description{
Read a DIC or DICX dictionary file directly into R.
}
\examples{
\dontrun{
# Read a dictionary file (Uruguay 2011, downloaded from redatam.org)
read_redatam("CP2011URY/BaseRPub/CPV2011_uruguay_publica.dicX")
}
}
