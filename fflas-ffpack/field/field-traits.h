/* -*- mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
// vim:sts=8:sw=8:ts=8:noet:sr:cino=>s,f0,{0,g0,(0,\:0,t0,+0,=s
/*
 * Copyright (C) 2014 the FFLAS-FFPACK group
 *
 * Written by Clement Pernet <Clement.Pernet@imag.fr>
 * BB <brice.boyer@lip6.fr>
 *
 *
 * ========LICENCE========
 * This file is part of the library FFLAS-FFPACK.
 *
 * FFLAS-FFPACK is free software: you can redistribute it and/or modify
 * it under the terms of the  GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * ========LICENCE========
 *.
 */

/** @file field/field-traits.h
 * @brief  Field Traits
 */

#ifndef __FFLASFFPACK_field_field_traits_H
#define __FFLASFFPACK_field_field_traits_H


namespace FFLAS {

	//! Traits and categories will need to be placed in a proper file later
	namespace FieldCategories {
		//! generic ring.
		struct GenericTag{};
		//! If it can init/convert elements to/from floating point types: float, double
		struct FloatingPointConvertibleTag : public  GenericTag{};
		//! If it is a Modular or ModularBalanced templated by float or double
		struct ModularFloatingPointTag : public GenericTag{};
		//! If it is a Modular or ModularBalanced templated by float or double, and result is not reduced
		struct DelayedModularFloatingPointTag : public GenericTag{};
		//! If it is a multiprecision field
		struct MultiPrecisionTag : public  GenericTag{};
		//! If it is DoubleDomain or a FloatDomain
		struct FloatingPointTag : public GenericTag{};

		//! This is a Modular<T> or ModularBalanced<T>
		struct ModularTag : public GenericTag {} ;
		//! This is an UnparametricField<T>
		struct UnparametricTag : public GenericTag {} ;
	}

} // FFLAS


namespace FFLAS {

	/*! FieldTrait
	*/
	template <class Field>
	struct FieldTraits {
		typedef typename FieldCategories::GenericTag value;
		typedef typename FieldCategories::GenericTag category;
	};

	// Modular <double|float>
	// ModularBalanced <double|float>
	template<>
	struct FieldTraits<FFPACK::Modular<double> > {
		typedef  FieldCategories::ModularFloatingPointTag value;
		typedef typename FieldCategories::ModularTag category;
	};
	template<>
	struct FieldTraits<FFPACK::Modular<float> > {
		typedef FieldCategories::ModularFloatingPointTag value;
		typedef typename FieldCategories::ModularTag category;
	};
	template<>
	struct FieldTraits<FFPACK::ModularBalanced<double> > {
		typedef FieldCategories::ModularFloatingPointTag value;
		typedef typename FieldCategories::ModularTag category;
	};
	template<>
	struct FieldTraits<FFPACK::ModularBalanced<float> > {
		typedef FieldCategories::ModularFloatingPointTag value;
		typedef typename FieldCategories::ModularTag category;
	};



	// Modular< intX >
	// ModularBalanced < intX >
	template<>
	struct FieldTraits<FFPACK::Modular<int32_t> > {
		typedef FieldCategories::FloatingPointConvertibleTag value;
		typedef typename FieldCategories::ModularTag category;
	};
	template<>
	struct FieldTraits<FFPACK::Modular<uint32_t> > {
		typedef FieldCategories::FloatingPointConvertibleTag value;
		typedef typename FieldCategories::ModularTag category;
	};
	template<>
	struct FieldTraits<FFPACK::Modular<int64_t> > {
		typedef FieldCategories::FloatingPointConvertibleTag value;
		typedef typename FieldCategories::ModularTag category;
	};
	template<>
	struct FieldTraits<FFPACK::Modular<uint64_t> > {
		typedef FieldCategories::FloatingPointConvertibleTag value;
		typedef typename FieldCategories::ModularTag category;
	};
	template<>
	struct FieldTraits<FFPACK::Modular<int16_t> > {
		typedef FieldCategories::FloatingPointConvertibleTag value;
		typedef typename FieldCategories::ModularTag category;
	};
	template<>
	struct FieldTraits<FFPACK::Modular<uint16_t> > {
		typedef FieldCategories::FloatingPointConvertibleTag value;
		typedef typename FieldCategories::ModularTag category;
	};
	template<>
	struct FieldTraits<FFPACK::ModularBalanced<int32_t> > {
		typedef FieldCategories::FloatingPointConvertibleTag value;
		typedef typename FieldCategories::ModularTag category;
	};
	template<>
	struct FieldTraits<FFPACK::ModularBalanced<uint32_t> > {
		typedef FieldCategories::FloatingPointConvertibleTag value;
		typedef typename FieldCategories::ModularTag category;
	};
	template<>
	struct FieldTraits<FFPACK::ModularBalanced<int64_t> > {
		typedef FieldCategories::FloatingPointConvertibleTag value;
		typedef typename FieldCategories::ModularTag category;
	};
	template<>
	struct FieldTraits<FFPACK::ModularBalanced<uint64_t> > {
		typedef FieldCategories::FloatingPointConvertibleTag value;
		typedef typename FieldCategories::ModularTag category;
	};
	template<>
	struct FieldTraits<FFPACK::ModularBalanced<int16_t> > {
		typedef FieldCategories::FloatingPointConvertibleTag value;
		typedef typename FieldCategories::ModularTag category;
	};
	template<>
	struct FieldTraits<FFPACK::ModularBalanced<uint16_t> > {
		typedef FieldCategories::FloatingPointConvertibleTag value;
		typedef typename FieldCategories::ModularTag category;
	};


	// UnparametricField<float| double>
	template<>
	struct FieldTraits<DoubleDomain> {
		typedef FieldCategories::FloatingPointTag value;
		typedef typename FieldCategories::UnparametricTag category;
	};
	template<>
	struct FieldTraits<FloatDomain> {
		typedef FieldCategories::FloatingPointTag value;
		typedef typename FieldCategories::UnparametricTag category;
	};


	// UnparametricField< intX >
	template<>
	struct FieldTraits<FFPACK::UnparametricField<int32_t> > {
		typedef FieldCategories::FloatingPointConvertibleTag value;
		typedef typename FieldCategories::UnparametricTag category;
	};
	template<>
	struct FieldTraits<FFPACK::UnparametricField<uint32_t> > {
		typedef FieldCategories::FloatingPointConvertibleTag value;
		typedef typename FieldCategories::UnparametricTag category;
	};
	template<>
	struct FieldTraits<FFPACK::UnparametricField<int64_t> > {
		typedef FieldCategories::FloatingPointConvertibleTag value;
		typedef typename FieldCategories::UnparametricTag category;
	};
	template<>
	struct FieldTraits<FFPACK::UnparametricField<uint64_t> > {
		typedef FieldCategories::FloatingPointConvertibleTag value;
		typedef typename FieldCategories::UnparametricTag category;
	};
	template<>
	struct FieldTraits<FFPACK::UnparametricField<int16_t> > {
		typedef FieldCategories::FloatingPointConvertibleTag value;
		typedef typename FieldCategories::UnparametricTag category;
	};
	template<>
	struct FieldTraits<FFPACK::UnparametricField<uint16_t> > {
		typedef FieldCategories::FloatingPointConvertibleTag value;
		typedef typename FieldCategories::UnparametricTag category;
	};

	//template<> struct FieldTraits<Modular<integer> > {typedef FieldCategories::MultiPrecisionTag value;};


} // FFLAS

namespace FFLAS { /* associatedDelayedField */

	template <class Field>
	struct associatedDelayedField{
		typedef Field field;
		typedef Field& type; // reference to avoid copying heavy fields
	};
	template <>
	struct associatedDelayedField<const FFPACK::Modular<float> >{
		typedef FloatDomain field;
		typedef FloatDomain type;
	};
	template <>
	struct associatedDelayedField<const FFPACK::ModularBalanced<float> >{
		typedef FloatDomain field;
		typedef FloatDomain type;
	};
	template <>
	struct associatedDelayedField<const FFPACK::Modular<double> >{
		typedef DoubleDomain field;
		typedef DoubleDomain type;
	};
	template <>
	struct associatedDelayedField<const FFPACK::ModularBalanced<double> >{
		typedef DoubleDomain field;
		typedef DoubleDomain type;
	};

} // FFLAS

#endif // __FFLASFFPACK_field_field_traits_H

