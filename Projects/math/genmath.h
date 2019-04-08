#pragma once

namespace mth
{
	typedef unsigned int uint;

	template <typename T, uint N>
	class Vector;

	template <typename T, uint R, uint C>
	class Matrix;


	template <typename T, uint N>
	class Vector
	{
		T m_v[N];

	public:
		Vector()
		{
			for (uint i = 0; i < N; i++)
				m_v[i] = 0;
		}
		Vector(const T f)
		{
			for (uint i = 0; i < N; i++)
				m_v[i] = f;
		}
		Vector(const T * const v)
		{
			for (uint i = 0; i < N; i++)
				m_v[i] = v[i];
		}
		Vector(const Vector<T, N>& v)
		{
			for (uint i = 0; i < N; i++)
				m_v[i] = v.m_v[i];
		}
		void Print()
		{
			for (uint n = 0; n < N; n++)
				std::cout << (abs(m_v[n]) < 1e-5 ? 0 : m_v[n]) << std::endl;
			std::cout << std::endl;
		}
		T Dot(const Vector<T, N> v) const
		{
			T dot = 0;
			for (uint i = 0; i < N; i++)
				dot += m_v[i] * v.m_v[i];
			return dot;
		}
		T LengthSquare() const
		{
			T s = 0;
			for (uint i = 0; i < N; i++)
				s += m_v[i] * m_v[i];
			return s;
		}
		T Length() const
		{
			return sqrt(LengthSquare());
		}
		void Normalize()
		{
			*this /= Length();
		}
		uint Size()
		{
			return N;
		}
		Vector<T, N> Normalized() const
		{
			return *this / Length();
		}
		Matrix<T, N, 1> ToColMatrix() const
		{
			return Matrix<T, N, 1>(m_v);
		}
		Matrix<T, 1, N> ToRowMatrix() const
		{
			return Matrix<T, 1, N>(m_v);
		}
		Vector<T, N> operator+(const Vector<T, N> v) const
		{
			Vector<T, N> r;
			for (uint i = 0; i < N; i++)
				r.m_v[i] = m_v[i] + v.m_v[i];
			return r;
		}
		Vector<T, N> operator-(const Vector<T, N> v) const
		{
			Vector<T, N> r;
			for (uint i = 0; i < N; i++)
				r.m_v[i] = m_v[i] - v.m_v[i];
			return r;
		}
		Vector<T, N> operator*(const Vector<T, N> v) const
		{
			Vector<T, N> r;
			for (uint i = 0; i < N; i++)
				r.m_v[i] = m_v[i] * v.m_v[i];
			return r;
		}
		Vector<T, N> operator/(const Vector<T, N> v) const
		{
			Vector<T, N> r;
			for (uint i = 0; i < N; i++)
				r.m_v[i] = m_v[i] / v.m_v[i];
			return r;
		}
		Vector<T, N> operator+(const T f) const
		{
			Vector<T, N> r;
			for (uint i = 0; i < N; i++)
				r.m_v[i] = m_v[i] + f;
			return r;
		}
		Vector<T, N> operator-(const T f) const
		{
			Vector<T, N> r;
			for (uint i = 0; i < N; i++)
				r.m_v[i] = m_v[i] - f;
			return r;
		}
		Vector<T, N> operator*(const T f) const
		{
			Vector<T, N> r;
			for (uint i = 0; i < N; i++)
				r.m_v[i] = m_v[i] * f;
			return r;
		}
		Vector<T, N> operator/(const T f) const
		{
			Vector<T, N> r;
			for (uint i = 0; i < N; i++)
				r.m_v[i] = m_v[i] / f;
			return r;
		}
		Vector<T, N>& operator+=(const Vector<T, N> v)
		{
			for (uint i = 0; i < N; i++)
				m_v[i] += v.m_v[i];
			return *this;
		}
		Vector<T, N>& operator-=(const Vector<T, N> v)
		{
			for (uint i = 0; i < N; i++)
				m_v[i] -= v.m_v[i];
			return *this;
		}
		Vector<T, N>& operator*=(const Vector<T, N> v)
		{
			for (uint i = 0; i < N; i++)
				m_v[i] *= v.m_v[i];
			return *this;
		}
		Vector<T, N>& operator/=(const Vector<T, N> v)
		{
			for (uint i = 0; i < N; i++)
				m_v[i] /= v.m_v[i];
			return *this;
		}
		Vector<T, N>& operator=(const Vector<T, N> v)
		{
			for (uint i = 0; i < N; i++)
				m_v[i] = v.m_v[i];
			return *this;
		}
		Vector<T, N>& operator+=(const T f)
		{
			for (uint i = 0; i < N; i++)
				m_v[i] += f;
			return *this;
		}
		Vector<T, N>& operator-=(const T f)
		{
			for (uint i = 0; i < N; i++)
				m_v[i] -= f;
			return *this;
		}
		Vector<T, N>& operator*=(const T f)
		{
			for (uint i = 0; i < N; i++)
				m_v[i] *= f;
			return *this;
		}
		Vector<T, N>& operator/=(const T f)
		{
			for (uint i = 0; i < N; i++)
				m_v[i] /= f;
			return *this;
		}
		Vector<T, N>& operator=(const T f)
		{
			for (uint i = 0; i < N; i++)
				m_v[i] = f;
			return *this;
		}
		template <uint C>
		Vector<T, C> operator*(const Matrix<T, N, C> m) const
		{
			Vector<T, C> q;
			for (uint c = 0; c < C; c++)
				for (uint i = 0; i < N; i++)
					q(c) += m(i, c) * m_v[i];
			return q;
		}
		T operator()(uint i) const
		{
			return m_v[i];
		}
		T& operator()(uint i)
		{
			return m_v[i];
		}
		operator T*()
		{
			return m_v;
		}
	};


	template <typename T, uint R, uint C>
	class Matrix
	{
		T m_m[R][C];

	public:
		Matrix()
		{
			for (uint r = 0; r < R; r++)
				for (uint c = 0; c < C; c++)
					m_m[r][c] = 0;
		}
		Matrix(const T f)
		{
			for (uint r = 0; r < R; r++)
				for (uint c = 0; c < C; c++)
					m_m[r][c] = f;
		}
		Matrix(const T * const m)
		{
			for (uint r = 0; r < R; r++)
				for (uint c = 0; c < C; c++)
					m_m[r][c] = m[r  * C+ c];
		}
		Matrix(const T * const * const m)
		{
			for (uint r = 0; r < R; r++)
				for (uint c = 0; c < C; c++)
					m_m[r][c] = m[r][c];
		}
		Matrix(const Matrix<T, R, C>& m)
		{
			for (uint r = 0; r < R; r++)
				for (uint c = 0; c < C; c++)
					m_m[r][c] = m.m_m[r][c];
		}
		Matrix(const Matrix<T, R, C>* m)
		{
			for (uint r = 0; r < R; r++)
				for (uint c = 0; c < C; c++)
					m_m[r][c] = m->m_m[r][c];
		}
		static Matrix<T, R, C> Identity()
		{
			Matrix<T, R, C> m;
			for (uint i = 0; i < (R < C ? R : C); i++)
				m.m_m[i][i] = 1;
			return m;
		}
		Matrix<T, R - 1, C - 1> SubMatrix(uint excludedRow, uint excludedColumn) const
		{
			Matrix<T, R - 1, C - 1> m;
			uint currrow;
			uint currcol;
			currrow = 0;
			for (uint r = 0; r < R; r++)
			{
				if (r != excludedRow)
				{
					currcol = 0;
					for (uint c = 0; c < C; c++)
					{
						if (c != excludedColumn)
						{
							m(currrow, currcol) = m_m[r][c];
							currcol++;
						}
					}
					currrow++;
				}
			}
			return m;
		}
		uint Rows()
		{
			return R;
		}
		uint Columns()
		{
			return C;
		}
		void Print()
		{
			for (uint r = 0; r < R; r++)
			{
				for (uint c = 0; c < C; c++)
				{
					std::cout << (abs(m_m[r][c]) < 1e-5 ? 0 : m_m[r][c]) << '\t';
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
		Vector<T, C> ColToVector(uint c = 0)
		{
			T col[C];
			for (uint i = 0; i < C; i++)
				col[i] = m_m[i][c];
			return Vector<T, C>(col);
		}
		Vector<T, R> RowToVector(uint r = 0)
		{
			T row[R];
			for (uint i = 0; i < R; i++)
				row[i] = m_m[r][i];
			return Vector<T, R>(row);
		}
		T operator()(const uint row, const uint column) const
		{
			return m_m[row][column];
		}
		T& operator()(const uint row, const uint column)
		{
			return m_m[row][column];
		}
		Vector<T, R> operator*(const Vector<T, C> v) const
		{
			Vector<T, R> q;
			for (uint r = 0; r < R; r++)
				for (uint i = 0; i < C; i++)
					q(r) += m_m[r][i] * v(i);
			return q;
		}
		T Determinant() const
		{
			if (R != C)
				throw std::exception("non-square matrix has no determinant");
			T d = 0;
			for (int r = 0; r < R; r++)
			{
				T tmp = m_m[r][0] * SubMatrix(r, 0).Determinant();
				if (r & 0x1)
					d -= tmp;
				else
					d += tmp;
			}
			return d;
		}
		Matrix<T, C, R> Inverse() const
		{
			Matrix<T, C, R> m;
			T oneoverdet = 1 / Determinant();
			for (uint c = 0; c < C; c++)
			{
				for (uint r = 0; r < R; r++)
				{
					T tmp = SubMatrix(r, c).Determinant() * oneoverdet;
					m(c, r) = (r + c & 0x1) ? -tmp : tmp;
				}
			}
			return m;
		}
		Matrix<T, C, R> Trasposed() const
		{
			Matrix<T, C, R> m;
			for (int c = 0; c < C; c++)
				for (int r = 0; r < R; r++)
					m(c, r) = m_m[r][c];
			return m;
		}
		Matrix<T, R, R> operator*(const Matrix<T, C, R>& m) const
		{
			Matrix<T, R, R> q;
			for (uint c = 0; c < R; c++)
				for (uint r = 0; r < R; r++)
					for (uint i = 0; i < C; i++)
						q(r, c) += m_m[r][i] * m(i, c);
			return q;
		}
		Matrix<T, R, C> operator+(const Matrix<T, R, C>& m) const
		{
			Matrix<T, R, C> q;
			for (uint c = 0; c < C; c++)
				for (uint r = 0; r < R; r++)
					q(r, c) = m_m[r][c] + m(r, c);
			return q;
		}
		Matrix<T, R, C>& operator+=(const Matrix<T, R, C>& m)
		{
			for (uint c = 0; c < C; c++)
				for (uint r = 0; r < R; r++)
					m_m[r][c] += m(r, c);
			return *this;
		}
		Matrix<T, R, C> operator-(const Matrix<T, R, C>& m) const
		{
			Matrix<T, R, C> q;
			for (uint c = 0; c < C; c++)
				for (uint r = 0; r < R; r++)
					q(r, c) = m_m[r][c] - m(r, c);
			return q;
		}
		Matrix<T, R, C>& operator-=(const Matrix<T, R, C>& m)
		{
			for (uint c = 0; c < C; c++)
				for (uint r = 0; r < R; r++)
					m_m[r][c] -= m(r, c);
			return *this;
		}
		Matrix<T, R, C>& operator=(const Matrix<T, R, C>& m)
		{
			for (uint r = 0; r < R; r++)
				for (uint c = 0; c < C; c++)
					m_m[r][c] = m.m_m[r][c];
			return *this;
		}
		Matrix<T, R, C> operator+(const T f) const
		{
			Matrix<T, R, C> m;
			for (uint r = 0; r < R; r++)
				for (uint c = 0; c < C; c++)
					m(c, r) = m_m[r][c] + f;
			return m;
		}
		Matrix<T, R, C>& operator+=(const T f)
		{
			for (uint r = 0; r < R; r++)
				for (uint c = 0; c < C; c++)
					m_m[r][c] += f;
			return *this;
		}
		Matrix<T, R, C> operator-(const T f) const
		{
			Matrix<T, R, C> m;
			for (uint r = 0; r < R; r++)
				for (uint c = 0; c < C; c++)
					m(c, r) = m_m[r][c] - f;
			return m;
		}
		Matrix<T, R, C>& operator-=(const T f)
		{
			for (uint r = 0; r < R; r++)
				for (uint c = 0; c < C; c++)
					m_m[r][c] -= f;
			return *this;
		}
		Matrix<T, R, C> operator*(const T f) const
		{
			Matrix<T, R, C> m;
			for (uint r = 0; r < R; r++)
				for (uint c = 0; c < C; c++)
					m(c, r) = m_m[r][c] * f;
			return m;
		}
		Matrix<T, R, C>& operator*=(const T f)
		{
			for (uint r = 0; r < R; r++)
				for (uint c = 0; c < C; c++)
					m_m[r][c] *= f;
			return *this;
		}
		Matrix<T, R, C> operator/(const T f) const
		{
			Matrix<T, R, C> m;
			for (uint r = 0; r < R; r++)
				for (uint c = 0; c < C; c++)
					m(c, r) = m_m[r][c] / +f;
			return m;
		}
		Matrix<T, R, C>& operator/=(const T f)
		{
			for (uint r = 0; r < R; r++)
				for (uint c = 0; c < C; c++)
					m_m[r][c] /= f;
			return *this;
		}
		Matrix<T, R, C>& operator=(const T f)
		{
			for (uint r = 0; r < R; r++)
				for (uint c = 0; c < C; c++)
					m_m[r][c] = f;
			return *this;
		}
	};

	template <typename T, uint R, uint C>
	Matrix<T, R, C> operator*(const Matrix<T, R, C> m, const T f)
	{
		return m * f;
	}
	template <typename T, uint N>
	Vector<T, N> operator*(const Vector<T, N> v, const T f)
	{
		return v * f;
	}

	float Matrix<float, 1, 1>::Determinant() const { return m_m[0][0]; }
	double Matrix<double, 1, 1>::Determinant() const { return m_m[0][0]; }
}