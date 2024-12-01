#pragma once


class BaseWindow
{
	public:
	virtual bool Init() = 0;
	virtual void Update() = 0;
	virtual void Clear() = 0;
	virtual bool ShouldClose() const = 0;
	virtual ~BaseWindow() = default;


	protected:

};

