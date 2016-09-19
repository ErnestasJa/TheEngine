template <typename T> inline std::string to_str(const T& t)
{
	std::ostringstream os;    //magic..
	os << t;
	return os.str();
}

template <typename T> inline std::wstring to_wstr(const T& t)
{
	std::wostringstream os;    //magic..
	os << t;
	return os.str();
}

inline std::string toHex(uint8_t value)
{
	auto n = glm::max<int>(0, glm::min<int>(value, 255));

	std::string hexstring = "0123456789ABCDEF";
	auto char1 = hexstring.at((n - n % 16) / 16);
	auto char2 = hexstring.at(n % 16);

	std::string resultString = "";
	resultString += char1;
	resultString += char2;

	return resultString;
}

inline std::wstring toHex_w(uint8_t value)
{
	auto n = glm::max<int>(0, glm::min<int>(value, 255));

	std::wstring hexstring = L"0123456789ABCDEF";
	auto char1 = hexstring.at((n - n % 16) / 16);
	auto char2 = hexstring.at(n % 16);

	std::wstring resultString = L"";
	resultString += char1;
	resultString += char2;

	return resultString;
}

template <typename T>
inline T limit(T val, T min, T max)
{
	T ret;
	if (val<min)
		ret = min;
	else if (val>max)
		ret = max;
	else
		ret = val;

	return ret;
}

inline const glm::vec4 color255(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	return glm::vec4((float)r / 255, (float)g / 255, (float)b / 255, (float)a / 255);
}

inline void invert(glm::vec3 & vec)
{
	if (vec.x)
		vec.x = 1.0f / vec.x;
	if (vec.y)
		vec.y = 1.0f / vec.y;
	if (vec.z)
		vec.z = 1.0f / vec.z;
}

template <>
inline bool equals(const float & v1, const float & v2)
{
	return (v1 + ROUNDING_ERROR >= v2) && (v1 - ROUNDING_ERROR <= v2);
}

template <>
inline bool equals(const glm::vec3 & v1, const glm::vec3 & v2)
{
	return equals(v1.x, v2.x) && equals(v1.y, v2.y) && equals(v1.z, v2.z);
}

template <>
inline bool equals(const glm::vec4 & v1, const glm::vec4 & v2)
{
	return equals(v1.x, v2.x) && equals(v1.y, v2.y) && equals(v1.z, v2.z) && equals(v1.w, v2.w);
}

template <>
inline bool equals(const glm::quat & v1, const glm::quat & v2)
{
	return equals(v1.x, v2.x) && equals(v1.y, v2.y) && equals(v1.z, v2.z) && equals(v1.w, v2.w);
}

template <>
inline bool equals(const glm::mat4 & v1, const glm::mat4 & v2)
{
	return equals(v1[0], v2[0]) && equals(v1[1], v2[1]) && equals(v1[2], v2[2]) && equals(v1[3], v2[3]);
}

template <>
inline bool equals(const glm::mat3 & v1, const glm::mat3 & v2)
{
	return equals(v1[0], v2[0]) && equals(v1[1], v2[1]) && equals(v1[2], v2[2]);
}

inline float coslerp(float y1, float y2, float mu)
{
	float mu2;

	mu2 = (1 - glm::cos(mu*glm::pi<float>())) / 2;
	return(y1*(1 - mu2) + y2*mu2);
}

inline glm::vec3 hsv2rgb(float h, float s, float v)
{
	float r, g, b;

	int i;
	float f, p, q, t;
	if (s == 0)
	{
		// achromatic (grey)
		r = g = b = v;
		return glm::vec3(r, g, b);
	}
	h /= 60;			// sector 0 to 5
	i = (int)floor(h);
	f = h - i;			// factorial part of h
	p = v * (1 - s);
	q = v * (1 - s * f);
	t = v * (1 - s * (1 - f));
	switch (i)
	{
	case 0:
		r = v;
		g = t;
		b = p;
		break;
	case 1:
		r = q;
		g = v;
		b = p;
		break;
	case 2:
		r = p;
		g = v;
		b = t;
		break;
	case 3:
		r = p;
		g = q;
		b = v;
		break;
	case 4:
		r = t;
		g = p;
		b = v;
		break;
	default:		// case 5:
		r = v;
		g = p;
		b = q;
		break;
	}
	return glm::vec3(r, g, b);
}

inline glm::vec3 rgb2hsv(float r, float g, float b)
{
	float h, s, v;

	float min, max, delta;
	min = glm::min(glm::min(r, g), b);
	max = glm::max(glm::max(r, g), b);
	v = max;				// v
	delta = max - min;
	if (max != 0)
		s = delta / max;		// s
	else
	{
		// r = g = b = 0		// s = 0, v is undefined
		s = 0;
		h = -1;
		return glm::vec3(0);
	}
	if (r == max)
		h = (g - b) / delta;		// between yellow & magenta
	else if (g == max)
		h = 2 + (b - r) / delta;	// between cyan & yellow
	else
		h = 4 + (r - g) / delta;	// between magenta & cyan
	h *= 60;				// degrees
	if (h < 0)
		h += 360;

	return glm::vec3(h, s, v);
}

inline glm::vec3 VectorMin(const glm::vec3 &vec1, const glm::vec3 &vec2)
{
	return glm::vec3(glm::min(vec1.x, vec2.x), glm::min(vec1.y, vec2.y), glm::min(vec1.z, vec2.z));
}
inline glm::vec3 VectorMax(const glm::vec3 &vec1, const glm::vec3 &vec2)
{
	return glm::vec3(glm::max(vec1.x, vec2.x), glm::max(vec1.y, vec2.y), glm::max(vec1.z, vec2.z));
}

inline float MakePOT(float v)
{
	return glm::pow(2.f, glm::ceil(glm::log((v)) / glm::log(2.f)));
}