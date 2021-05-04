#ifndef CATMACRO_H_
#define CATMACRO_H_
#define	FNTYPE(ATI,NTI,LTI)	BL ## ATI ## NTI ## LTI ## _ft
#define	FNNAME(FNBASE,NTI,LTI)	BL ## FNBASE ## NTI ## LTI
#define	STTYPE(STBASE,NTI,LTI)	BL ## STBASE ## NTI ## LTI ## _t
#define	PSTTYPE(STBASE,NTI,LTI)	pBL ## STBASE ## NTI ## LTI ## _t
#define	CPSTTYPE(STBASE,NTI,LTI)	pcBL ## STBASE ## NTI ## LTI ## _t
#define IDXRM(icol,irow,nc)     (icol + irow * nc)
#define IDXCM(icol,irow,nr)     (irow + icol * nr)
#endif
