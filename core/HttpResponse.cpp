// Copyright (c) 2014 The SealedServer Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "HttpRequest.h"
#include "HttpResponse.h"

namespace sealedserver
{

static const char *status_code_to_str(int status_code) {
    switch (status_code) {
    case 200:
        return "OK";
    case 201:
        return "Created";
    case 204:
        return "No Content";
    case 301:
        return "Moved Permanently";
    case 302:
        return "Found";
    case 304:
        return "Not Modified";
    case 400:
        return "Bad Request";
    case 403:
        return "Forbidden";
    case 404:
        return "Not Found";
    case 405:
        return "Method Not Allowed";
    case 409:
        return "Conflict";
    case 411:
        return "Length Required";
    case 413:
        return "Request Entity Too Large";
    case 415:
        return "Unsupported Media Type";
    case 423:
        return "Locked";
    case 500:
        return "Server Error";
    case 501:
        return "Not Implemented";
    default:
        return "Server Error";
    }
}

static string getPrefix(string url)
{
    /// TBD
    int index = url.find("http://");
    if(index != -1) url = url.substr(index + 7);
        index = url.find("/");
    if(index != -1) url = url.substr(0, index);

    return url;
}

static string getPosix(string url)
{
    /// TBD
    int index = url.find("http://");
    if(index != -1) url = url.substr(index + 7);
        index = url.find("/");
    if(index != -1) url = url.substr(index + 1);

    return url;
}

/// For the server
HttpResponse::HttpResponse(HttpRequest *request, int code) : request_(request)
{
    stringstream ss;
    ss << "HTTP/1.1 " << code << " " << status_code_to_str(code) << "\r\n";

    header_ = ss.str();
}

/// For the client
HttpResponse::HttpResponse(HttpRequest *request) : request_(request)
{
    stringstream ss; 
    ss << "GET " << getPosix(request -> getUrl()) << " " << "HTTP/1.1" << "\r\n";

    header_ = ss.str();

    addHeader("Host", getPrefix(request -> getUrl()));
    addHeader("User-Agent", "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:24.0) Gecko/20100101 Firefox/24.0");
    addHeader("Accept-Language", "en-US,en;q=0.5");
    addHeader("Accept-Encoding", "gzip, deflate");
}

HttpResponse::~HttpResponse()
{
}

void HttpResponse::send()
{
    string content = header_ + "\r\n" + body_;

    request_ -> send(content);
}

void HttpResponse::addHeader(const string &key, const string &value)
{
    stringstream ss;
    ss << key << ": " << value << "\r\n";

    header_ += ss.str();
}

void HttpResponse::addBody(const string &body)
{
    body_ += body;
}

};