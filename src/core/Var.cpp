#include "Precomp.h"
#include "Var.h"

uint32_t allocAndCopyStr(char *& dest, const char * orig)
{
	uint32_t len = orig ? strlen(orig) + 1 : 0;

	if (len)
	{
		dest = new char[len];
		strcpy(dest, orig);
	}
	else
		dest = nullptr;

	return len;
}

Var Var::Null;

Var::Var()
{
	m_type = VARNULL;
	m_count = 0;
	m_name = nullptr;
	m_flags = 0;
}

Var::Var(const Var & o)
{
	if (o.Type() == VARNULL)
	{
		m_count = 0;
		m_type = VARNULL;
		m_name = nullptr;
		m_flags = 0;
		return;
	}
	else if (o.Name() == nullptr)
	{
		throw std::string("Variable not null type, but name is null");
	}

	allocAndCopyStr(m_name, o.Name());
	m_type = o.Type();
	m_count = o.Count();
	m_flags = o.Flags();

	if (m_count)
		switch (m_type)
		{
		case VARI:
		{
			if (m_count == 1)
				m_datai = o.ValueI();
			else
				Value(o.ValueIV(), o.Count());
			break;
		}
		case VARF:
		{
			if (m_count == 1)
				m_dataf = o.ValueF();
			else
				Value(o.ValueFV(), o.Count());
			break;
		}
		case VARS:
		{
			allocAndCopyStr(m_datas, o.ValueS());
			break;
		}
		}
}

Var & Var::operator = (const Var & other)
{
	(*this) = Var(other);
	return *this;
}

///rename value to name
Var::Var(const char * name, float data)
{
	allocAndCopyStr(m_name, name);
	m_type = VARF;
	m_dataf = data;
	m_count = 1;
	m_flags = 0;
}

Var::Var(const char * name, float * data, uint8_t count)
{
	allocAndCopyStr(m_name, name);
	m_type = VARF;
	m_datafv = new float[count];
	std::copy(data, data + count, m_datafv);
	m_count = count;
	m_flags = 0;
}

Var::Var(const char * name, int data)
{
	allocAndCopyStr(m_name, name);
	m_type = VARI;
	m_datai = data;
	m_count = 1;
	m_flags = 0;
}

Var::Var(const char * name, int * data, uint8_t count)
{
	allocAndCopyStr(m_name, name);
	m_type = VARF;
	m_dataiv = new int[count];
	std::copy(data, data + count, m_dataiv);
	m_count = count;
	m_flags = 0;
}

Var::Var(const char * name, const char * data)
{
	allocAndCopyStr(m_name, name);
	m_type = VARS;
	m_count = allocAndCopyStr(m_datas, data);
	m_flags = 0;
}

Var::Var(const char * name, const std::string & data)
{
	allocAndCopyStr(m_name, name);
	m_type = VARS;
	m_count = allocAndCopyStr(m_datas, data.c_str());
	m_flags = 0;
}

Var::~Var()
{
	if (m_name)
		delete[] m_name;

	if (m_count)
		switch (m_type)
		{
		case VARF:
			if (m_count > 1)
				delete[] m_datafv;
			break;
		case VARI:
			if (m_count > 1)
				delete[] m_dataiv;
			break;
		case VARS:
			delete[] m_datas;
			break;
		default:
			throw std::string("Bad var type");
		}
}

VarType Var::Type() const
{
	return (VarType)m_type;
}

uint8_t Var::Flags() const
{
	return m_flags;
}

uint8_t Var::Count() const
{
	return m_count;
}

const char * Var::Name() const
{
	return m_name;
}

int Var::ValueI() const
{
	return m_datai;
}

float Var::ValueF() const
{
	return m_dataf;
}

int * Var::ValueIV() const
{
	return m_dataiv;
}

float * Var::ValueFV() const
{
	return m_datafv;
}

const char * Var::ValueS() const
{
	return m_datas;
}


void Var::AppendFlags(uint8_t flags)
{
	m_flags |= flags;
}

void Var::SetFlags(uint8_t flags)
{
	m_flags = flags;
}

void Var::RemoveFlags(uint8_t flags)
{
	m_flags = m_flags & ~flags;
}


void Var::Value(int * values, uint8_t count)
{
	///optimize realocs
	if (m_dataiv)
		delete[] m_dataiv;

	m_dataiv = new int[count];
	std::copy(values, values + count, m_dataiv);
	m_count = count;
}

void Var::Value(float * values, uint8_t count)
{
	///optimize realocs
	if (m_datafv)
		delete[] m_datafv;

	m_datafv = new float[count];
	std::copy(values, values + count, m_datafv);
	m_count = count;
}

void Var::Value(float value)
{
	m_dataf = value;
}

void Var::Value(int value)
{
	m_datai = value;
}

void Var::Value(const std::string & value)
{
	if (m_datas)
		delete[] m_datas;

	allocAndCopyStr(m_datas, value.c_str());
}