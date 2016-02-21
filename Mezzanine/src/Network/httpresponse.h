// © Copyright 2010 - 2016 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/

#ifndef _networkhttpresponse_h
#define _networkhttpresponse_h

#include "version.h"
#include "Network/networkenumerations.h"
#include "Network/httpmessage.h"

namespace Mezzanine
{
    namespace Network
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class HTTPResponse
        /// @brief
        ///////////////////////////////////////
        class MEZZ_LIB HTTPResponse : public HTTPMessage
        {
        protected:
            /// @internal
            /// @brief The HTTP method to use for the response.
            Whole ResponseCode;
            /// @internal
            /// @brief The textual message delivered to explain the status code.
            String ResponseDescription;

            /// @copydoc HTTPMessage::ParseHTTPHeader(StringIterator&,const StringIterator)
            Boole ParseHTTPHeader(StringIterator& CurrIt, const StringIterator EndIt);
        public:
            /// @brief Class constructor.
            HTTPResponse();
            /// @brief String constructor.
            /// @param ToDecompose The String to construct this response from.
            HTTPResponse(const String& ToDecompose);
            /// @brief Class destructor.
            ~HTTPResponse();

            ///////////////////////////////////////////////////////////////////////////////
            // Core Operations

            /// @copydoc HTTPMessage::Compose() const
            String Compose() const;
            /// @copydoc HTTPMessage::Decompose(const String&)
            Boole Decompose(const String& Response);

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Sets the status code explaining the result of a HTTP request.
            /// @param Code The HTTP status code representing the manner of response to a previous HTTP request.
            void SetStatusCode(const Whole Code);
            /// @brief Gets the status code explaining the result of a HTTP request.
            /// @return Returns a HTTPStatusCode enum value explaining the result of a previous HTTP request.
            Whole GetStatusCode() const;

            /// @brief Sets the textual description to explain the status code contained in the response.
            /// @param Message A String to explain the provided status code.
            void SetResponseDescription(const String& Message);
            /// @brief Gets the textual description to explain the status code contained in the response.
            /// @return Returns a String containing the description of the status code.
            const String& GetResponseDescription() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Recommended Header Field Convenience Methods

            /// @brief Sets the Allow Header.
            /// @remarks This header is required to be defined in a response when the server responds with "405 Method not allowed".
            /// @param Allow A comma separated list of HTTP methods that are allowed to be performed on the on the resource located on the responding server.
            void SetAllowHeader(const String& Allow);
            /// @brief Gets the Allow Header.
            /// @return Returns a String containing a comma separated list of supported HTTP methods allowed to be used on the specified resource, or blank if the field is not set.
            const String& GetAllowHeader() const;
            /// @brief Sets the Content-Encoding Header.
            /// @remarks This header is required if any kind of encoding is performed on the message body of the response.
            /// @param ContentEncoding A description of the encoding used on the message body if it were encoded/compressed.
            void SetContentEncodingHeader(const String& ContentEncoding);
            /// @brief Gets the Content-Encoding Header.
            /// @return Returns a String containing a description of the encoding used on the message body if it were encoded/compressed, or blank if the field is not set.
            const String& GetContentEncodingHeader() const;
            /// @brief Sets the Date Header.
            /// @remarks A server must provide a Date header if it is capable of providing a reasonable approximation of the actual time.
            /// @param Date The time and date of when the server processed the request in UTC.
            void SetDateHeader(const String& Date);
            /// @brief Gets the Date Header.
            /// @return Returns a String containing the time and date of when the server processed the request in UTC, or blank if the field is not set.
            const String& GetDateHeader() const;
            /// @brief Sets the Server Header.
            /// @remarks An optional but widely used header field to describe what software is running on the server.
            /// @param Server Information on the platform/server being used by the server.
            void SetServerHeader(const String& Server);
            /// @brief Gets the Server Header.
            /// @return Returns a String containing information on the platform/server being used by the server, or blank if the field is not set.
            const String& GetServerHeader() const;
        };//HTTPResponse
    }//Network
}//Mezzanine

#endif
