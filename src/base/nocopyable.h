#ifndef _NOCOPYABLE_H_
#define _NOCOPYABLE_H_

class nocopyable 
{
protected:
	nocopyable() = default;
	~nocopyable() = default;
private:
	nocopyable(const nocopyable&) = delete;
	const nocopyable& operator=(const nocopyable&) = delete;
};
#endif
