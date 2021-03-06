/* Copyright (C) 2007 LinBox
 * Written by <Jean-Guillaume.Dumas@imag.fr>
 *
 *
 *
 * ========LICENCE========
 * This file is part of the library LinBox.
 *
  * LinBox is free software: you can redistribute it and/or modify
 * it under the terms of the  GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * ========LICENCE========
 */

#ifndef __LINBOX_rational_early_single_cra_H
#define __LINBOX_rational_early_single_cra_H

#include "givaro/zring.h"
#include "linbox/algorithms/cra-early-single.h"

namespace LinBox
{

	template<class Domain_Type>
	struct EarlySingleRatCRA : public EarlySingleCRA<Domain_Type> {
		typedef Domain_Type				Domain;
		typedef EarlySingleCRA<Domain> 			Father_t;
		typedef typename Father_t::DomainElement 	DomainElement;
		typedef EarlySingleRatCRA<Domain>		Self_t;
		Givaro::ZRing<Integer> _ZZ;

		Integer					Numer0;
		Integer					Denom0;

	public:

		EarlySingleRatCRA(const unsigned long EARLY=DEFAULT_EARLY_TERM_THRESHOLD) :
			Father_t(EARLY)
		{}

		void progress (const Domain& D, const DomainElement& e)
		{
			DomainElement u0, m0;

			fieldreconstruct(this->residue_, D, e, D.init(u0,this->residue_), D.init(m0,this->primeProd_), Integer(this->residue_), this->primeProd_);
			D.characteristic( this->nextM_ );
			this->primeProd_ *= this->nextM_;
			Integer a, b;
			_ZZ.reconstructRational(a, b, this->residue_, this->primeProd_);
			if ((a == Numer0) && (b == Denom0))
				++this->occurency_;
			else {
				this->occurency_ = 1;
				Numer0 = a;
				Denom0 = b;
			}
		}

		void progress (const Integer & D, const Integer & e)
		{
			Integer u0 = this->residue_   %D;
			Integer	m0 = this->primeProd_ %D;

			fieldreconstruct(this->residue_, D, e, u0, m0, Integer(this->residue_), this->primeProd_);
			this->primeProd_ *= this->nextM_;
			Integer a, b;
			_ZZ.reconstructRational(a, b, this->residue_, this->primeProd_);
			if ((a == Numer0) && (b == Denom0))
				++this->occurency_;
			else {
				this->occurency_ = 1;
				Numer0 = a;
				Denom0 = b;
			}
		}


		void initialize (const Domain& D, const DomainElement& e)
		{
			Father_t::initialize(D, e);
			_ZZ.reconstructRational(Numer0, Denom0, this->residue_, this->primeProd_);
		}

		void initialize (const Integer& D, const Integer& e)
	       	{
			Father_t::initialize(D, e);
			_ZZ.reconstructRational(Numer0, Denom0, this->residue_, this->primeProd_);
		}

		Integer& result(Integer& Num, Integer& Den)
		{
			Den = Denom0;
			return Num=Numer0;
		}

		Integer& result(Integer& d)
		{
			throw "not a good idea calling this function here !!" ;
		}
	protected:

		Integer& fieldreconstruct(Integer& res, const Domain& D1,
					  const DomainElement& u1, DomainElement& u0,
					  DomainElement& m0, const Integer& r0,
					  const Integer& P0)
		{
			// u0 and m0 are modified
			D1.negin(u0);   	// u0 <-- -u0
			D1.addin(u0,u1);	// u0 <-- u1-u0
			D1.invin(m0);	// m0 <-- m0^{-1} mod m1
			D1.mulin(u0, m0);   // u0 <-- (u1-u0)( m0^{-1} mod m1 )
			D1.convert(res, u0);// res <-- (u1-u0)( m0^{-1} mod m1 )         and res <  m1
			res *= P0;      	// res <-- (u1-u0)( m0^{-1} mod m1 ) m0      and res <= (m0m1-m0)
			return res += r0;   // res <-- u0 + (u1-u0)( m0^{-1} mod m1 ) m0 and res <  m0m1
		}

		Integer& fieldreconstruct(Integer& res, const Integer& D,
					  const Integer & u1, Integer & u0,
					  Integer & m0, const Integer& r0,
					  const Integer& P0)
		{
			// u0 and m0 are modified
			inv(m0,m0,D);           // m0 <-- m0^{-1} mod m1
			u0 = (u1-u0) ;
			u0 *= m0 ;
			u0 %= D  ;              // u0 <-- (u1-u0)( m0^{-1} mod m1 )
			res = u0*P0 ;        	// res <-- (u1-u0)( m0^{-1} mod m1 ) m0      and res <= (m0m1-m0)
			return res += r0;   // res <-- u0 + (u1-u0)( m0^{-1} mod m1 ) m0 and res <  m0m1
		}
	};
}

#endif // __LINBOX_rational_early_single_cra_H

// Local Variables:
// mode: C++
// tab-width: 4
// indent-tabs-mode: nil
// c-basic-offset: 4
// End:
// vim:sts=4:sw=4:ts=4:et:sr:cino=>s,f0,{0,g0,(0,\:0,t0,+0,=s
