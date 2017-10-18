/*----------------- HttpProcessor.cpp
*
* Copyright (C): 2016  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2016/8/27 11:28:12
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "HttpProcessor.h"
#include "HttpClient.h"
#include "curl/curl.h"

typedef int int32_t;
typedef size_t (*write_callback)(void *ptr, size_t size, size_t nmemb, void *stream);
//-------------------------------------------------------------
//------------------------------  Callback function used by libcurl for collect pResponse data
static size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream)
{
	std::vector<char> *recvBuffer = (std::vector<char>*)stream;
	size_t sizes = size * nmemb;

	// add data to the end of recvBuffer
	// write data maybe called more than once in a single pRequest
	recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+sizes);

	return sizes;
}
//-------------------------------------------------------------
//------------------------------  Callback function used by libcurl for collect header data
static size_t writeHeaderData(void *ptr, size_t size, size_t nmemb, void *stream)
{
	std::vector<char> *recvBuffer = (std::vector<char>*)stream;
	size_t sizes = size * nmemb;

	// add data to the end of recvBuffer
	// write data maybe called more than once in a single pRequest
	recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+sizes);

	return sizes;
}
//-------------------------------------------------------------
//------------------------------ 
static int processGetTask(HttpProcessor* pProcessor,HttpRequest* pRequest, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream, char* errorBuffer);
static int processPostTask(HttpProcessor* pProcessor,HttpRequest* pRequest, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream, char* errorBuffer);
static int processPutTask(HttpProcessor* pProcessor,HttpRequest* pRequest, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream, char* errorBuffer);
static int processDeleteTask(HttpProcessor* pProcessor,HttpRequest* pRequest, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream, char* errorBuffer);
//-------------------------------------------------------------
//------------------------------ Configure curl's timeout property
static bool configureCURL(HttpProcessor* pProcessor,CURL* handle, char* errorBuffer)
{
    if (!handle || !pProcessor ) {
        return false;
    }
    
    int32_t code;
    code = curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, errorBuffer);
    if (code != CURLE_OK) {
        return false;
    }
    code = curl_easy_setopt(handle, CURLOPT_TIMEOUT,pProcessor->getClient().getTimeoutForRead() );
    if (code != CURLE_OK) {
        return false;
    }
    code = curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, pProcessor->getClient().getTimeoutForConnect());
    if (code != CURLE_OK) {
        return false;
    }


    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0L);

    
    // FIXED #3224: The subthread of CCHttpClient interrupts main thread if timeout comes.
    // Document is here: http://curl.haxx.se/libcurl/c/curl_easy_setopt.html#CURLOPTNOSIGNAL 
    curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1L);

    curl_easy_setopt(handle, CURLOPT_ACCEPT_ENCODING, "");

    return true;
}
//-------------------------------------------------------------
//------------------------------ 
class CURLRaii
{
    /// Instance of CURL
    CURL *_curl;
    /// Keeps custom header data
    curl_slist *_headers;
public:
    CURLRaii()
        : _curl(curl_easy_init())
        , _headers(NULL)
    {
    }

    ~CURLRaii()
    {
        if (_curl)
            curl_easy_cleanup(_curl);
        /* free the linked list for header data */
        if (_headers)
            curl_slist_free_all(_headers);
    }

    template <class T>
    bool setOption(CURLoption option, T data)
    {
        return CURLE_OK == curl_easy_setopt(_curl, option, data);
    }

    /**
     * @brief Inits CURL instance for common usage
     * @param pRequest Null not allowed
     * @param callback Response write callback
     * @param stream Response write stream
     */
    bool init(HttpProcessor* pProcessor,HttpRequest* pRequest, write_callback callback, void* stream, write_callback headerCallback, void* headerStream, char* errorBuffer)
    {
        if (!_curl)
            return false;
		if (!configureCURL(pProcessor,_curl, errorBuffer))
            return false;

        /* get custom header data (if set) */
        std::vector<std::string> headers=pRequest->getHeaders();
        if(!headers.empty())
        {
            /* append custom headers one by one */
            for (std::vector<std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
                _headers = curl_slist_append(_headers,it->c_str());
            /* set custom headers for curl */
            if (!setOption(CURLOPT_HTTPHEADER, _headers))
                return false;
        }
        return setOption(CURLOPT_URL, pRequest->getUrl())
                && setOption(CURLOPT_WRITEFUNCTION, callback)
                && setOption(CURLOPT_WRITEDATA, stream)
                && setOption(CURLOPT_HEADERFUNCTION, headerCallback)
                && setOption(CURLOPT_HEADERDATA, headerStream);
        
    }

    /// @param responseCode Null not allowed
    bool perform(long *responseCode)
    {
        if (CURLE_OK != curl_easy_perform(_curl))
            return false;
        CURLcode code = curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, responseCode);
        if (code != CURLE_OK || !(*responseCode >= 200 && *responseCode < 300)) {
            //LOG(ERROR_LEVEL, "Curl curl_easy_getinfo failed: %s", curl_easy_strerror(code));
            return false;
        }
        // Get some mor data.
        
        return true;
    }
};
//-------------------------------------------------------------
//------------------------------ Process Get Request
static int processGetTask(HttpProcessor* pProcessor,HttpRequest* pRequest, write_callback callback, void* stream, long* responseCode, write_callback headerCallback, void* headerStream, char* errorBuffer)
{
    CURLRaii curl;
	bool ok = curl.init(pProcessor,pRequest, callback, stream, headerCallback, headerStream, errorBuffer)
            && curl.setOption(CURLOPT_FOLLOWLOCATION, true)
			&& curl.setOption(CURLOPT_COOKIE, pRequest->getCookieData())
            && curl.perform(responseCode);
    return ok ? 0 : 1;
}
//-------------------------------------------------------------
//------------------------------ Process POST Request
static int processPostTask(HttpProcessor* pProcessor,HttpRequest* pRequest, write_callback callback, void* stream, long* responseCode, write_callback headerCallback, void* headerStream, char* errorBuffer)
{
    CURLRaii curl;
	bool ok = curl.init(pProcessor,pRequest, callback, stream, headerCallback, headerStream, errorBuffer)
            && curl.setOption(CURLOPT_POST, 1)
            && curl.setOption(CURLOPT_POSTFIELDS, pRequest->getRequestData())
            && curl.setOption(CURLOPT_POSTFIELDSIZE, pRequest->getRequestDataSize())
			&& curl.setOption(CURLOPT_COOKIE, pRequest->getCookieData())
            && curl.perform(responseCode);
    return ok ? 0 : 1;
}
//-------------------------------------------------------------
//------------------------------ Process PUT Request
static int processPutTask(HttpProcessor* pProcessor,HttpRequest* pRequest, write_callback callback, void* stream, long* responseCode, write_callback headerCallback, void* headerStream, char* errorBuffer)
{
    CURLRaii curl;
	bool ok = curl.init(pProcessor,pRequest, callback, stream, headerCallback, headerStream, errorBuffer)
            && curl.setOption(CURLOPT_CUSTOMREQUEST, "PUT")
            && curl.setOption(CURLOPT_POSTFIELDS, pRequest->getRequestData())
            && curl.setOption(CURLOPT_POSTFIELDSIZE, pRequest->getRequestDataSize())
            && curl.perform(responseCode);
    return ok ? 0 : 1;
}
//-------------------------------------------------------------
//------------------------------ Process DELETE Request
static int processDeleteTask(HttpProcessor* pProcessor,HttpRequest* pRequest, write_callback callback, void* stream, long* responseCode, write_callback headerCallback, void* headerStream, char* errorBuffer)
{
    CURLRaii curl;
	bool ok = curl.init(pProcessor,pRequest, callback, stream, headerCallback, headerStream, errorBuffer)
            && curl.setOption(CURLOPT_CUSTOMREQUEST, "DELETE")
            && curl.setOption(CURLOPT_FOLLOWLOCATION, true)
            && curl.perform(responseCode);
    return ok ? 0 : 1;
}
//-------------------------------------------------------------
//------------------------------ 
HttpProcessor::HttpProcessor( HttpClient & clClient)
	:	m_clClient ( clClient )
{
}
//-------------------------------------------------------------
//------------------------------ 
HttpProcessor::~HttpProcessor(void)
{
}
//-------------------------------------------------------------
//------------------------------ 
void	HttpProcessor::process()
{
	while(!m_clClient.m_clExitEvent.wait_event(50) && !m_clClient.m_bShutdown )
	{
		if (m_clClient.m_listRequest.isEmpty())
			continue;

		HttpRequest* pRequest = m_clClient.m_listRequest.pop();
		if (pRequest )
		{
			HttpResponse* pResponse = processRequest(pRequest);
			m_clClient.postHttpResponse(pResponse);
		}
	}
}
//-------------------------------------------------------------
//------------------------------ 
HttpResponse* HttpProcessor::processRequest(HttpRequest* pRequest)
{
	if ( !pRequest )
		return NULL;

	HttpResponse *pResponse = NULL;
	try
	{
		long responseCode = -1;
		int retValue = 0;

		pResponse = new (std::nothrow) HttpResponse(pRequest);
		memset(m_responseMessage, 0, HttpProcessor::RESPONSE_BUFFER_SIZE);

		// Process the pRequest -> get pResponse packet
		switch (pRequest->getRequestType())
		{
		case HttpRequest::GET: // HTTP GET
			retValue = processGetTask(this,pRequest,
				writeData,
				pResponse->getResponseData(),
				&responseCode,
				writeHeaderData,
				pResponse->getResponseHeader(),
				m_responseMessage);
			break;

		case HttpRequest::POST: // HTTP POST
			retValue = processPostTask(this,pRequest,
				writeData,
				pResponse->getResponseData(),
				&responseCode,
				writeHeaderData,
				pResponse->getResponseHeader(),
				m_responseMessage);
			break;

		case HttpRequest::PUT:
			retValue = processPutTask(this,pRequest,
				writeData,
				pResponse->getResponseData(),
				&responseCode,
				writeHeaderData,
				pResponse->getResponseHeader(),
				m_responseMessage);
			break;

		case HttpRequest::REMOVE:
			retValue = processDeleteTask(this,pRequest,
				writeData,
				pResponse->getResponseData(),
				&responseCode,
				writeHeaderData,
				pResponse->getResponseHeader(),
				m_responseMessage);
			break;

		default:
			break;
		}

		// write data to HttpResponse
		pResponse->setResponseCode(responseCode);
		if (retValue != 0)
		{
			pResponse->setSucceed(false);
			pResponse->setErrorBuffer(m_responseMessage);
		}
		else
		{
			pResponse->setSucceed(true);
		}
	}
	catch (...)
	{
	}

	return pResponse;
}