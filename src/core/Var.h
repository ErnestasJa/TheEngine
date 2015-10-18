#ifndef VAR_H
#define VAR_H

enum VarType
{
	VARNULL = 0,
	VARI = 1,
	VARF,
	VARS
};

enum VarFlag
{
	VAR_NOFLAGS = 0,
	VAR_RUNTIME = 1,
	//VAR_READONLY
};

uint32_t allocAndCopyStr(char *& dest, const char * orig);

class Var
{
public:
	static Var Null;

public:
	Var();
	Var(const Var & o);
	Var & operator = (const Var & other);

	explicit Var(const char * name, float data);
	explicit Var(const char * name, float * data, uint8_t count);
	explicit Var(const char * name, int data);
	explicit Var(const char * name, int * data, uint8_t count);
	explicit Var(const char * name, const char * data);
	explicit Var(const char * name, const std::string & data);
	~Var();
	VarType Type() const;
	uint8_t Flags() const;
	uint8_t Count() const;
	const char * Name() const;
	int ValueI() const;
	float ValueF() const;
	int* ValueIV() const;
	float* ValueFV() const;
	const char * ValueS() const;

	void AppendFlags(uint8_t flags);
	void SetFlags(uint8_t flags);
	void RemoveFlags(uint8_t flags);
	void Value(int value);
	void Value(float value);
	void Value(int* values, uint8_t count);
	void Value(float* values, uint8_t count);
	void Value(const char* value);
	void Value(const std::string & value);

private:
	char * m_name;
	uint8_t m_count;
	uint8_t m_type;
	uint8_t m_flags;

	union
	{
		char *    m_datas;
		float *   m_datafv;
		int32_t * m_dataiv;
		float     m_dataf;
		int32_t   m_datai;
	};
};

#endif
