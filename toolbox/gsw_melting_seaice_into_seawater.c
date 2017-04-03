/*
!==========================================================================
elemental subroutine gsw_melting_seaice_into_seawater (sa, ct, p, &
                         w_seaice, sa_seaice, t_seaice, sa_final, ct_final)
!==========================================================================
!
!  Calculates the Absolute Salinity and Conservative Temperature that 
!  results when a given mass of sea ice (or ice) melts and is mixed into a 
!  known mass of seawater (whose properties are (SA,CT,p)).  
!
!  If the ice contains no salt (e.g. if it is of glacial origin), then the 
!  input 'SA_seaice' should be set to zero.  
!
!  Ice formed at the sea surface (sea ice) typically contains between 2 g/kg
!  and 12 g/kg of salt (defined as the mass of salt divided by the mass of 
!  ice Ih plus brine) and this programme returns NaN's if the input  
!  SA_seaice is greater than 15 g/kg.  If the SA_seaice input is not zero,   
!  usually this would imply that the pressure p should be zero, as sea ice  
!  only occurs near the sea surface.  The code does not impose that p = 0 
!  if SA_seaice is non-zero.  Rather, this is left to the user.  
!
!  The Absolute Salinity, SA_brine, of the brine trapped in little pockets 
!  in the sea ice, is in thermodynamic equilibrium with the ice Ih that
!  surrounds these pockets.  As the sea ice temperature, t_seaice, may be 
!  less than the freezing temperature, SA_brine is usually greater than the
!  Absolute Salinity of the seawater at the time and place when and where 
!  the sea ice was formed.  So usually SA_brine will be larger than SA.  
!
!  SA  =  Absolute Salinity of seawater                            [ g/kg ]
!  CT  =  Conservative Temperature of seawater (ITS-90)           [ deg C ]
!  p   =  sea pressure at which the melting occurs                 [ dbar ]
!         ( i.e. absolute pressure - 10.1325 dbar ) 
!  w_seaice  =  mass fraction of sea ice, that is the mass of sea ice 
!               divided by the sum of the masses of sea ice and seawater. 
!               That is, the mass of sea ice divided by the mass of the 
!               final mixed fluid.  w_seaice must be between 0 and 1. 
!                                                              [ unitless ]
!  SA_seaice =  Absolute Salinity of sea ice, that is, the mass fraction of
!               salt in sea ice, expressed in g of salt per kg of sea ice.
!                                                                  [ g/kg ]
!  t_seaice  =  the in-situ temperature of the sea ice (or ice) (ITS-90)
!                                                                 [ deg C ]
!
!  SA_final  =  Absolute Salinity of the mixture of the melted sea ice 
!               (or ice) and the orignal seawater                  [ g/kg ]
!  CT_final  =  Conservative Temperature of the mixture of the melted 
!               sea ice (or ice) and the orignal seawater         [ deg C ]
!--------------------------------------------------------------------------
*/
void
gsw_melting_seaice_into_seawater(double sa, double ct, double p,
	double w_seaice, double sa_seaice, double t_seaice,
	double *sa_final, double *ct_final)
{
	double	ctf, h, h_brine, h_final, h_ih, sa_brine, tf_sa_seaice;
	double	saturation_fraction = 0.0;

	ctf = gsw_ct_freezing(sa,p,saturation_fraction);
	if (ct < ctf) {
	    /*The seawater ct input is below the freezing temp*/
	    *sa_final = GSW_INVALID_VALUE;
	    *ct_final = *sa_final;
	    return;
	}

	tf_sa_seaice = gsw_t_freezing(sa_seaice,p,saturation_fraction)
			- 1e-6;
	if (t_seaice > tf_sa_seaice) {
	/*
	! The 1e-6 C buffer in the allowable t_seaice is to ensure that there is
	! some ice Ih in the sea ice. Without this buffer, that is if t_seaice
	! is allowed to be exactly equal to tf_sa_seaice, the seaice is 
	! actually 100% brine at Absolute Salinity of SA_seaice.
	*/
	    *sa_final = GSW_INVALID_VALUE;
	    *ct_final = *sa_final;
	    return;
	}

	sa_brine = gsw_sa_freezing_from_t(t_seaice,p,saturation_fraction);
	if (sa_brine >= GSW_ERROR_LIMIT) {
	    *sa_final = GSW_INVALID_VALUE;
	    *ct_final = *sa_final;
	    return;
	}
	h_brine = gsw_enthalpy_t_exact(sa_brine,t_seaice,p);

	h = gsw_enthalpy_ct_exact(sa,ct,p);
	h_ih = gsw_enthalpy_ice(t_seaice,p);

	h_final = h - w_seaice*(h - h_ih - (h_brine - h_ih)*sa_seaice/sa_brine);

	*sa_final = sa - w_seaice*(sa - sa_seaice);
	/*
	!ctf = gsw_ct_freezing(sa_final,p,saturation_fraction)
	!
	!if (h_final .lt. gsw_enthalpy_ct_exact(sa_final,ctf,p)) then
	!    ! Melting this much seaice is not possible as it would result in
	!    ! frozen seawater
	!    sa_final = gsw_error_code(4,func_name)
	!    ct_final = sa_final
	!    return
	!end if
	*/
	*ct_final = gsw_ct_from_enthalpy_exact(*sa_final,h_final,p);
	if (*ct_final > GSW_ERROR_LIMIT) {
	    *sa_final = *ct_final;
	    return;
	}
}
