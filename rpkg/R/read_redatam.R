#' @title Read REDATAM dictionary
#' @description Read a DIC or DICX dictionary file directly into R.
#' @param dictionary A character string with the path to the dictionary file. It
#'  allows relative paths.
#' @return A list of tibbles, each one representing a table with the census
#'  entities (or levels) and their attributes (or variables).
#' @examples
#' # Read a dictionary file (Galapagos 2015)
#' zip <- system.file("extdata", "galapagos.zip", package = "redatam")
#' dout <- paste(tempdir(), "galapagos", sep = "/")
#' unzip(zip, exdir = dout)
#' read_redatam(paste(dout, "cg15.dic", sep = "/"))
#' @export
read_redatam <- function(dictionary) {
  dictionary <- normalizePath(dictionary)

  if (grepl("\\.dic$|\\.DIC$|\\.dicx$|\\.DICX$||\\.dicX", dictionary)) {
    res <- export_redatam_to_list_(dictionary)
  } else {
    stop("The file extension must be either .dic or .dicx")
  }

  message("Fixing encoding...")

  # 1. convert lists to data.tables
  res <- list_to_datatable_(res)
  res <- res[vapply(res, function(x) nrow(x) > 0, logical(1))]

  # 2. fix encoding (e.g., ""C\xf3digo Region" -> "Código Region")
  res <- fix_encoding_(res)

  message("Tidying names...")

  # 3. convert REGION, Region, Regi\u00f3n, etc. to region and so on
  res <- tidy_names_(res)

  message("Tidying variables...")

  # 4. remove leading and trailing whitespaces and multiple whitespaces
  res <- trim_and_clean_(res)

  # 6. harmonize column types
  res <- harmonize_types_(res)

  # 7. merge with their descriptions
  res <- merge_descriptions_(res)
  
  # 8. show as tibble
  res <- datatable_to_tibble_(res)

  # 9. put entities first
  res <- res[sort(names(res))]

  return(res)
}
