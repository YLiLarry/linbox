/*
 * Copyright (C) the LinBox group
 *
 * Written by BB <bbboyer@ncsu.edu>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * ========LICENCE========
 *.
 */

#ifndef __LINBOX_matrix_sparse_matrix_read_write_sparse_INL
#define __LINBOX_matrix_sparse_matrix_read_write_sparse_INL

// read/write functions
namespace LinBox {

	/**** Read ***/
	template<class Matrix>
	std::istream &SparseMatrixReadWriteHelper<Matrix>::readTurner (Matrix &A, std::istream &is
				  , char *buf)
	{
		size_t i, j;

		A.getRep().clear ();
		A.getRep().resize (A.rowdim());

		do {
			std::istringstream str (buf);

			str >> i;

			if (i == (size_t) -1) break; // return also if row index is -1
			str >> j;
			A.field().read (str, A.refEntry (i, j));

			is.getline (buf, 80);
		} while (is);

		return is;
	}

	template<class Matrix>
	std::istream &SparseMatrixReadWriteHelper<Matrix>::readGuillaume (Matrix &A, std::istream &is
				     , char *buf)
	{
		typedef typename Matrix::Field::Element Element;
		size_t i = 0, j = 0 ;

		std::istringstream str (buf);
		str >> A._m >> A._n;

		A.getRep().clear ();
		A.getRep().resize (A.rowdim());

		Element x;
		while (is >> i) {
			if (i == 0 || i == (size_t) -1) {is >> j; A.field().read(is, x); break;}
			is >> j;
			if (i > A.rowdim() || j > A.coldim())
				throw Exceptions::InvalidMatrixInput ();
			A.field().read (is, x);
			if (! A.field().isZero(x)) A.setEntry (i - 1, j - 1, x);
		}

		return is;

	}

	template<class Matrix>
	std::istream &SparseMatrixReadWriteHelper<Matrix>::readMatlab (Matrix &A, std::istream &is
				  , char *buf)
	{
		typedef typename Matrix::Field::Element Element;
		size_t i = 0, j = 0;
		char c;
		Element a_ij;

		while (1) {
			do is >> c; while (is && !std::isdigit (c));
			if (!is) break;

			is.putback (c);

			A.field().read (is, a_ij);
			A.setEntry (i, j++, a_ij);

			do is >> c; while (is && c != ',' && c != ';' && c != ']');
			if (!is) break;;

			if (c == ';') {
				++i;
				j = 0;
			}
			else if (c == ']') break;
		}

		return is;
	}

	template<class Matrix>
	std::istream &SparseMatrixReadWriteHelper<Matrix>::readPretty (Matrix &A, std::istream &is
				  , char *buf)
	{
		typedef typename Matrix::Field::Element Element;
		size_t i;
		Element a_ij;

		A._m = 0;
		A.getRep().clear ();

		i = 0;

		do {
			char c;
			size_t j;
			++(A._m);
			A.getRep().push_back (Matrix::Row ());

			std::istringstream str (buf);

			do {str >> c; std::cout << "x" << c << "x" << std::endl;} while (isspace (c));
			if (c != '[')
				throw Exceptions::InvalidMatrixInput ();

			j = 0;

			while (str) {
				do str >> c; while (isspace (c));
				if (!str || c == ']') break;
				A.field().read (str, a_ij);

				++j;
				if (j > A.coldim())
					++(A._n);

				if (!A.field().isZero(a_ij))
					A.setEntry (i, j, a_ij);
			}

			is.getline (buf, 80);

			++i;
		} while (is);

			return is;

	}

	template<class Matrix>
	std::istream &SparseMatrixReadWriteHelper<Matrix>::readMagmaCpt (Matrix &A, std::istream &is
				    , char *buf)
	{
		typedef typename Matrix::Field::Element Element;
		size_t i, j;
		Element a_ij;
		char c;
		const char matrixstart = '[', matrixend = ']';
		const char rowstart = '[', rowend = ']';
		const char pairstart = '[', pairend = ']';

		A._m = A._n = 0;
		A.getRep().clear ();

		do {is.get(c);} while (c != matrixstart ); // find matrix start
		i = 0;
		while (true)
		{
			do {is.get(c);} while (c != matrixend && c != rowstart);
			if (c == matrixend) return is;
			else
			{
				++(A._m);
				A.getRep().push_back (Matrix::Row ());
				//processrow(i)
				while (true)
				{
					do {is.get(c);} while (c != pairstart && c != rowend );
					if (c == rowend) break;
					else
					{  //processpair( j v for row i);
						is >> j;
						if (j > A.coldim()) A._n = j;
						do {is.get(c);} while (!std::isdigit(c) && c != '-' && c != '+');
						is.unget();
						A.field().read(is, a_ij);
						if (!A.field().isZero(a_ij)) A.setEntry (i, j-1, a_ij);
						do {is.get(c);} while (c != pairend);
					}
				}
				++i;
			}
		}
	}

	/**** Write ***/
	template<class Matrix>
	std::ostream &SparseMatrixWriteHelper<Matrix>::writeTriple (const Matrix &A, std::ostream &os, bool oneBased)
	{
		typename Matrix::Rep::const_iterator i;
		typename Matrix::Row::const_iterator j;
		size_t i_idx, j_idx;
		//	int col_width;
		integer c;
		// bool firstrow;


		// The i j v triples, with zero based indices.
		for (i = A.getRep().begin (), i_idx = 0; i != A.getRep().end (); ++i, ++i_idx) {
			for (j = i->begin (), j_idx = 0; j != i->end (); ++j, ++j_idx) {
				if (oneBased)
					os << i_idx + 1 << ' ' << j->first + 1 << ' ';
				else
					os << i_idx << ' ' << j->first << ' ';
				A.field().write (os, j->second);
				os << std::endl;
			}
		}
		return os;

	}

	template<class Matrix>
	std::ostream &SparseMatrixWriteHelper<Matrix>::writePretty (const Matrix &A, std::ostream &os
				   , std::string begmat
				   , std::string endmat
				   , std::string begrow
				   , std::string endrow
				   , std::string sepelt
				   , std::string seprow
				  )
	{
		typename Matrix::Rep::const_iterator i;
		typename Matrix::Row::const_iterator j;
		size_t i_idx, j_idx;
		//	int col_width;
		integer c;
		bool firstrow;

		os << begmat;
		firstrow=true;

		for (i = A.getRep().begin (), i_idx = 0; i != A.getRep().end (); ++i, ++i_idx) {
			j = i->begin ();

			if (firstrow) {
				os << begrow;
				firstrow =false;
			}
			else
				os << seprow << begrow;


			for (j_idx = 0; j_idx < A.coldim(); ++j_idx) {
				if (j == i->end () || j_idx != j->first)
					A.field().write (os, A.field().zero);
				else {
					A.field().write (os, j->second);
					++j;
				}

				if (j_idx < A.coldim() - 1)
					os << sepelt << ' ';
			}

			os << endrow;
		}

		os << endmat;

		return os ;
	}
} // namespace LinBox

namespace LinBox {
	// read
	template<class Matrix>
	std::istream &SparseMatrixReadWriteHelper<Matrix> ::read (Matrix &A, std::istream &is
								  , LINBOX_enum(Tag::FileFormat) format
								  , MatrixCategories::RowMatrixTag)
	{
		char buf[80];
		buf[0]=0;

		switch (format) {
		case Tag::FileFormat::Detect:
			{
				char c;
				is.getline (buf, 80);
				std::istringstream str (buf);
				do str >> c; while (isspace (c));

				if (c == '[') {
					if (strchr (buf, ';') != NULL)
						readMatlab (A, is, buf);
					else
						readPretty (A, is, buf);
				}
				else if (std::isdigit (c)) {
					do str >> c; while (str && (isspace (c) || std::isdigit (c)));

					if (c == 'M')
						return readGuillaume (A, is, buf);
					else
						return readTurner (A, is, buf);
				}
				else
					throw Exceptions::InvalidMatrixInput ();
				break;
			}

		case Tag::FileFormat::Turner:
			return readTurner (A, is, buf);
		case Tag::FileFormat::Guillaume:
			return readGuillaume (A, is, buf);
		case Tag::FileFormat::Matlab:
			return readMatlab (A, is, buf);
		case Tag::FileFormat::Pretty:
			return readPretty (A, is, buf);
		case Tag::FileFormat::MagmaCpt:
			return readMagmaCpt (A, is, buf);
		default:
			throw Exceptions::InvalidMatrixInput();
		}

		return is;
	}

	// write
	template<class Matrix>
	std::ostream &SparseMatrixWriteHelper<Matrix> ::write (const Matrix &A, std::ostream &os
							       , LINBOX_enum(Tag::FileFormat) format
							       , MatrixCategories::RowMatrixTag)
	{
		// Avoid massive unneeded overhead in the case that this
		// printing is disabled
		if (not os)
			return os;

		switch (format) {
		case Tag::FileFormat::Detect:
			throw PreconditionFailed (__func__, __LINE__, "format != Tag::FileFormat::Detect");

		case Tag::FileFormat::Turner:
			return writeTriple(A,os);

		case Tag::FileFormat::OneBased:
			return writeTriple(A,os,true);

		case Tag::FileFormat::Guillaume:
			// row col 'M' header line followed by the i j v triples, one based,
			// followed by 0 0 0.
			os << A.rowdim() << ' ' << A.coldim() << " M" << std::endl;
			writeTriple(A,os,true);
			os << "0 0 0" << std::endl;

			return os;

		case Tag::FileFormat::Matlab:
			return writePretty(A,os,"[","]","","; ",",","");
			// std::string begmat = "[";
			// std::string endmat = "]";
			// std::string begrow = "";
			// std::string endrow = "; ";
			// std::string sepelt  = ",";
			// std::string seprow  = "";

		case Tag::FileFormat::Maple:
			return writePretty(A,os,"[","]","["," ]",",",", ");
			// std::string begmat = "[";
			// std::string endmat = "]";
			// std::string begrow = "[";
			// std::string endrow = " ]";
			// std::string sepelt = ",";
			// std::string seprow = ", "

			break;

		case Tag::FileFormat::Pretty:
			return writePretty(A,os,"",""," [ ","]\n"," ","");
			// std::string begmat = "";
			// std::string endmat = "";
			// std::string begrow = " [ ";
			// std::string endrow = "]\n";
			// std::string sepelt  = " ";
			// std::string seprow  = "";


		case Tag::FileFormat::MagmaCpt:
			os << "sparse matrix written in MagmaCpt form is not implemented" << std::endl;
			break;

		default:
			os << "sparse matrix written in Tag::FileFormat::" << (int)format << " is not implemented" << std::endl;
		}

		return os;
	}

} // namespace LinBox

#endif // __LINBOX_matrix_sparse_matrix_read_write_sparse_INL


// Local Variables:
// mode: C++
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 8
// End:
// vim:sts=8:sw=8:ts=8:noet:sr:cino=>s,f0,{0,g0,(0,\:0,t0,+0,=s
