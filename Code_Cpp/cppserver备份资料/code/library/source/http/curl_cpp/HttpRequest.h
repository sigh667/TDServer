

#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

#include <string>
#include <vector>


class HttpClient;
class HttpResponse;

class HttpRequest 
{
public:
    /**
     * The HttpRequest type enum used in the HttpRequest::setRequestType.
     */
	enum _emRequestType
	{
		GET,
		POST,
		PUT,
		REMOVE,
		UNKNOWN,
	};
    
    /** 
     *  Constructor.
     *   Because HttpRequest object will be used between UI thead and network thread,
         requestObj->autorelease() is forbidden to avoid crashes in AutoreleasePool
         new/retain/release still works, which means you need to release it manually
         Please refer to HttpRequestTest.cpp to find its usage.
     */
    HttpRequest()
    {
        _requestType = UNKNOWN;
        _url.clear();
        _requestData.clear();
    };
    
    /** Destructor. */
    virtual ~HttpRequest()
    {
    };

	inline	void			initialize()
	{
		_requestType = UNKNOWN;
		_url.clear();
		_requestData.clear();
		_headers.clear();
		_cookieData.clear();
	}

            
    // setter/getters for properties
     
    /** 
     * Set request type of HttpRequest object before being sent,now it support the enum value of HttpRequest::Type.
     *
     * @param type the request type.
     */
    inline void setRequestType(int type)
    {
        _requestType = type;
    };
    /** 
     * Get the request type of HttpRequest object.
     *
     * @return HttpRequest::Type.
     */
    inline int getRequestType()
    {
        return _requestType;
    };
    
    /** 
     * Set the url address of HttpRequest object.
     * The url value could be like these: "http://httpbin.org/ip" or "https://httpbin.org/get"
     *
     * @param url the string pointer.
     */
    inline void setUrl(const char* url)
    {
        _url = url;
    };
    /** 
     * Get the url address of HttpRequest object.
     *
     * @return const char* the pointer of _url.
     */
    inline const char* getUrl()
    {
        return _url.c_str();
    };
    
    /** 
     * Set the request data of HttpRequest object.
     *
     * @param buffer the buffer of request data, it support binary data.
     * @param len    the size of request data.
     */
    inline void setRequestData(const char* buffer, size_t len)
    {
        _requestData.assign(buffer, buffer + len);
    };
    /** 
     * Get the request data pointer of HttpRequest object.
     *
     * @return char* the request data pointer.
     */
    inline char* getRequestData()
    {
        if(_requestData.size() != 0)
            return &(_requestData.front());

        return NULL;
    }
    /** 
     * Get the size of request data
     *
     * @return ssize_t the size of request data
     */
    inline size_t getRequestDataSize()
    {
        return _requestData.size();
    }
    /** 
     * Set custom-defined headers.
     *
     * @param pHeaders the string vector of custom-defined headers.
     */
    inline void setHeaders(std::vector<std::string> pHeaders)
   	{
   		_headers=pHeaders;
   	}
   
    /** 
     * Get custom headers.
     *
     * @return std::vector<std::string> the string vector of custom-defined headers.
     */
   	inline std::vector<std::string> getHeaders()
   	{
   		return _headers;
   	}

	inline void setCookieData(const char* buffer, size_t len)
	{
		_cookieData.assign(buffer, buffer + len);
	}

	inline char* getCookieData()
	{
		if(_cookieData.size() != 0)
			return &(_cookieData.front());

		return NULL;
	}
    
protected:
    // properties
    int							_requestType;    /// kHttpRequestGet, kHttpRequestPost or other enums
    std::string                 _url;            /// target url that this request is sent to
    std::vector<char>           _requestData;    /// used for POST
    std::vector<std::string>    _headers;		 /// custom http headers
	std::vector<char>           _cookieData;    //custom http cookie
};

#endif //__HTTP_REQUEST_H__
