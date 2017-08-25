#' dim
#' 
#' Dimension information for a float vector/matrix.
#' 
#' @param x
#' A float vector/matrix.
#' @param value
#' The right hand side for the "setter" (\code{dim<-}).
#' 
#' @return
#' The requested integer values.
#' 
#' @examples
#' \dontrun{
#' library(float)
#' 
#' s = flrunif(10, 3)
#' dim(s)
#' nrow(s)
#' ncol(s)
#' }
#' 
#' @name dims
#' @rdname dims
NULL



#' @rdname dims
#' @export
setMethod("nrow", signature(x="spm"), function(x) .Call(R_nrow_spm, x@ptr))

#' @rdname dims
#' @export
setMethod("ncol", signature(x="spm"), function(x) .Call(R_ncol_spm, x@ptr))

#' @rdname dims
#' @export
setMethod("NROW", signature(x="spm"), function(x) .Call(R_NROW_spm, x@ptr))

#' @rdname dims
#' @export
setMethod("NCOL", signature(x="spm"), function(x) .Call(R_NCOL_spm, x@ptr))

#' @rdname dims
#' @export
setMethod("dim", signature(x="spm"), function(x) .Call(R_dim_spm, x@ptr))

#' @rdname dims
#' @export
setMethod("length", signature(x="spm"), function(x) .Call(R_length_spm, x@ptr))

# for my eyes only
isavec = function(x) .Call(R_isavec_spm, x@ptr)



dimset.spm = function(x, value)
{
  ptr = .Call(R_dimset_spm, x@ptr, value)
  new("spm", ptr=ptr)
}

#' @rdname dims
#' @export
setReplaceMethod("dim", signature(x="spm"), dimset.spm)
