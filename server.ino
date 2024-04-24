/* Our URI handler function to be called during GET /uri request */
// the handlers are bound to a max of 8
// so we group them in 
// html_handler --> handle_pages
// params_handler --> handle_params
// actions_handler --> handle_actiona
// se we need 1 handler per group

 esp_err_t httpRequestAuthorization(httpd_req_t *req)
 {
    httpd_resp_set_hdr(req, "WWW-Authenticate", "Basic realm=\"my_realm1\"");
    httpd_resp_set_status(req, "401 Unauthorized");
    httpd_resp_set_type(req, HTTPD_TYPE_TEXT);
    httpd_resp_sendstr(req, "Unauthorized");
    return ESP_OK;
  }

// response on events open socket request

esp_err_t event_get_handler(httpd_req_t *req)
{ 
// this handle reponses to an events open socket request
// the response needs to have this format otherwise the
// socket is not opened

// response on events open socket request:
 char sse_resp[] = "HTTP/1.1 200 OK\r\n"
                               "Cache-Control: no-store\r\n"
                               "Content-Type: text/event-stream\r\n"
                               "\r\n"
                               "retry: 20000\r\n"
                               "\r\n";
   mySocketHD = req->handle;
   mySocketFD = httpd_req_to_sockfd(req);
   consoleOut("socket opened ?");
   httpd_socket_send(mySocketHD, mySocketFD, sse_resp, strlen(sse_resp), 0); 
   return ESP_OK;
}

esp_err_t html_handler(httpd_req_t *req)
{
// this handles all requests for webpages, stylesheets, etc except homepage
// handle_pages filters out what the request is for
    consoleOut("SHOWHTML requested");
    if(httpAuthenticateRequest_admin(req, "admin", pswd) == false ) {return httpRequestAuthorization(req);}
    handle_pages(req);
    return ESP_OK;
}

esp_err_t params_handler(httpd_req_t *req)
{
// handles all form submissions, handle params filters which form
// was submitted
    consoleOut("parameters requested");
    if(httpAuthenticateRequest_admin(req, "admin", pswd) == false ) {return httpRequestAuthorization(req);}
    handle_params(req);
    return ESP_OK;
}
esp_err_t actions_handler(httpd_req_t *req)
{
// handles all requests for actions like reboot, start AP, switch-on etc
    consoleOut("actions requested");
    handle_actions(req);
    return ESP_OK;
}

esp_err_t home_handler(httpd_req_t *req)
{
    consoleOut("homepage requested");
    if(httpAuthenticateRequest_admin(req, "admin", pswd) == false) return httpRequestAuthorization(req);
    httpd_resp_send(req, HOMEPAGE, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
  }

  esp_err_t data_handler(httpd_req_t *req)
  {
  // handles all datarequests
  // was submitted
      consoleOut("data requested");
      handle_data(req);
      return ESP_OK;
  }


    /* Function for starting the webserver */
    httpd_handle_t start_webserver(void)
    {
        /* Generate default configuration */
        httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    
    /* URI handler structure for GET / */
    httpd_uri_t home_get = {
        .uri      = "/",
        .method   = HTTP_GET,
        .handler  = home_handler,
        .user_ctx = NULL
    };
    /* URI handler structure for GET /SHOWHTML */
    httpd_uri_t html_get = {
        .uri      = "/SHOWHTML",
        .method   = HTTP_GET,
        .handler  = html_handler,
        .user_ctx = NULL
    };
    /* URI handler structure for GET /RUNACTION */
    httpd_uri_t actions_get = {
        .uri      = "/RUNACTION",
        .method   = HTTP_GET,
        .handler  = actions_handler,
        .user_ctx = NULL
    };
    
    /* URI handler structure for GET /getData */
    httpd_uri_t data_get = {
        .uri      = "/getData",
        .method   = HTTP_GET,
        .handler  = data_handler,
        .user_ctx = NULL
    };
    /* URI handler structure for GET /events */
    static const httpd_uri_t event_get = {
        .uri = "/events",
        .method = HTTP_GET,
        .handler = event_get_handler,
    };
    /* URI handler structure for GET /parameters */
    httpd_uri_t params_get = {
        .uri      = "/parameters",
        .method   = HTTP_GET,
        .handler  = params_handler,
        .user_ctx = NULL
    };
    
    /* Empty handle to esp_http_server */
    httpd_handle_t server = NULL;

    /* Start the httpd server */
    if (httpd_start(&server, &config) == ESP_OK) {
        /* Register URI handlers */
        httpd_register_uri_handler(server, &home_get);
        httpd_register_uri_handler(server, &data_get);
        httpd_register_uri_handler(server, &actions_get);
        httpd_register_uri_handler(server, &html_get);
        httpd_register_uri_handler(server, &params_get);
        httpd_register_uri_handler(server, &event_get);         
        // httpd_register_uri_handler(server, &uri_post);
    }
    /* If server failed to start, handle will be NULL */
    return server;
   }

/* Function for stopping the webserver */
void stop_webserver(httpd_handle_t server)
{
    if (server) {
        /* Stop the httpd server */
        httpd_stop(server);
    }
}



 esp_err_t wifihome_handler(httpd_req_t *req)
{
    consoleOut("portal homepage requested");
    handlePortalRoot(req);
    return ESP_OK;
}
  
esp_err_t wifiportal_handler(httpd_req_t *req)
{
// this handles all requests for webpages, stylesheets, etc except homepage
// handle_pages filters out what the request is for
    consoleOut("PORTAL requested");
    handle_wifiportal(req);
    return ESP_OK;
}
  
// httpd_uri_t wifiportal_get = {
//    //this is a general handler for all url with WIFIPORTAL
//    .uri      = "/PORTAL",
//    .method   = HTTP_GET,
//    .handler  = wifiportal_handler,
//    .user_ctx = NULL
//  };
//  
//  httpd_uri_t portalhome_get = {
//      .uri      = "/WIFIPORTAL",
//      .method   = HTTP_GET,
//      .handler  = wifihome_handler,
//      .user_ctx = NULL
//  }; 

  httpd_handle_t start_portalwebserver(void)
{
    /* Generate default configuration */
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

 httpd_uri_t wifiportal_get = {
    //this is a general handler for all url with WIFIPORTAL
    .uri      = "/PORTAL",
    .method   = HTTP_GET,
    .handler  = wifiportal_handler,
    .user_ctx = NULL
  };
  
  httpd_uri_t portalhome_get = {
      .uri      = "/WIFIPORTAL",
      .method   = HTTP_GET,
      .handler  = wifihome_handler,
      .user_ctx = NULL
  }; 
    
    /* Empty handle to esp_http_server */
    httpd_handle_t server = NULL;

    /* Start the httpd server */
    if (httpd_start(&server, &config) == ESP_OK) {
        /* Register URI handlers */
        httpd_register_uri_handler(server, &portalhome_get);
        httpd_register_uri_handler(server, &wifiportal_get);
    }
    /* If server failed to start, handle will be NULL */
    return server;
}

String decode_uri(String urlcode)
{
    String strcode = "";
    int i = 0;
    int len = urlcode.length();
    for (i = 0; i < len; ++i) 
    {
       // wdtFeed();
        char c = urlcode[i];
        if (c != '%') {
            strcode += String(c);
        } else {
            char c1 = urlcode[++i];
            char c0 = urlcode[++i];
            int num = 0;
            num = hex2dec(c1) * 16 + hex2dec(c0);
            strcode += String((char)num);
        }
    }
  return(strcode);
}
int hex2dec(char c){
    if ('0' <= c && c <= '9') { return c - '0'; } 
    else if ('a' <= c && c <= 'f') { return c - 'a' + 10; } 
    else if ('A' <= c && c <= 'F') { return c - 'A' + 10; } 
    else { return -1; }
}

bool httpAuthenticateRequest_admin(httpd_req_t *req, const char *server_username, const char *server_password)
{
consoleOut("httpAuthenticateRequest input :"); 
  consoleOut(String(server_username));
  consoleOut(String(server_password)); 
  char  authorization_header[64] = {0};
  char decoded_authorization[32] = {0};
    size_t buf_len;

  // Get header value string length
  buf_len = httpd_req_get_hdr_value_len(req, "Authorization");

  //ESP_LOGD(TAG, "Authorization header length %d", buf_len);
  //bound check
  if ((buf_len > 0) && (buf_len < 64))
  {
    // Copy null terminated value string into buffer
    if (httpd_req_get_hdr_value_str(req, "Authorization", authorization_header, buf_len + 1) == ESP_OK)
    {
      //ESP_LOGD(TAG, "Authorization header : %s", authorization_header);
      
      //find the "Basic " part of the header
      char *encoded_authorization = strstr(authorization_header, "Basic ");
      if(encoded_authorization != NULL)
      {
        //move the pointer to the start of the encoded authorization string
        encoded_authorization = &encoded_authorization[strlen("Basic ")];

        //ESP_LOGD(TAG, "Authorization string : %s", encoded_authorization);

        //decode the authorization string
        int decode_res = mbedtls_base64_decode((unsigned char *)decoded_authorization, sizeof(decoded_authorization), &buf_len, (unsigned char *)encoded_authorization, strlen(encoded_authorization));
        if(decode_res == 0)
        {
          //ESP_LOGD(TAG, "Decoded Authorization string : %s", decoded_authorization);

          //find the separator between username:password
          char *colon_index = strchr(decoded_authorization, ':');
          if(colon_index != NULL)
          {
            //replace colon index with null termination 
            colon_index[0] = 0;
            //username is from start till our previous null termination
            char *req_username = &decoded_authorization[0];
            //the rest is the password
            char *req_password = &colon_index[1];
            consoleOut("httpAuthenticateRequest req_username and req_password :"); 
            // in all cases we get the same data
            //Serial.println(String(server_username)); //admin
            //Serial.println(String(server_password)); // 1234
 //           Serial.println(String(req_username)); // user
 //           Serial.println(String(req_password)); // 1111
            //Serial.println(String(pswd));          // 1234
 //           Serial.println(String(userPwd));       // 1111                  
            //ESP_LOGD(TAG, "Username:%s, Password:%s", req_username, req_password);
            //char server_username[11] = "admin";
            //check if both username and password match the server's credentials
             if (strcmp(req_username, "admin") == 0 && strcmp(req_password, pswd) == 0) return true; 
            
            //Serial.println("no match found");
          }
          else
          {
            consoleOut("Decode authorization does not contain password");
          }
        }
        else
        {
          //ESP_LOGD(TAG, "Decoding failed");
        }
      }
      else
      {
        //ESP_LOGD(TAG, "Authorization value not in correct format");
      }
    }
    else
    {
      //ESP_LOGD(TAG, "Cannot retrieve autorization value");
    }
  }
  else
  {
    //ESP_LOGD(TAG, "No autorization header or too long");
  }
  
  //ESP_LOGW(TAG, "Authentication Failed");
  return false;
}

//bool httpAuthenticateRequest_user(httpd_req_t *req, const char *server_username, const char *server_password)
//{
//consoleOut("httpAuthenticateRequest input :"); 
//  Serial.println(String(server_username));
//  Serial.println(String(server_password)); 
//  char  authorization_header[64] = {0};
//  char decoded_authorization[32] = {0};
//    size_t buf_len;
//
//  // Get header value string length
//  buf_len = httpd_req_get_hdr_value_len(req, "Authorization");
//
//  //ESP_LOGD(TAG, "Authorization header length %d", buf_len);
//  //bound check
//  if ((buf_len > 0) && (buf_len < 64))
//  {
//    // Copy null terminated value string into buffer
//    if (httpd_req_get_hdr_value_str(req, "Authorization", authorization_header, buf_len + 1) == ESP_OK)
//    {
//      //ESP_LOGD(TAG, "Authorization header : %s", authorization_header);
//      
//      //find the "Basic " part of the header
//      char *encoded_authorization = strstr(authorization_header, "Basic ");
//      if(encoded_authorization != NULL)
//      {
//        //move the pointer to the start of the encoded authorization string
//        encoded_authorization = &encoded_authorization[strlen("Basic ")];
//
//        //ESP_LOGD(TAG, "Authorization string : %s", encoded_authorization);
//
//        //decode the authorization string
//        int decode_res = mbedtls_base64_decode((unsigned char *)decoded_authorization, sizeof(decoded_authorization), &buf_len, (unsigned char *)encoded_authorization, strlen(encoded_authorization));
//        if(decode_res == 0)
//        {
//          //ESP_LOGD(TAG, "Decoded Authorization string : %s", decoded_authorization);
//
//          //find the separator between username:password
//          char *colon_index = strchr(decoded_authorization, ':');
//          if(colon_index != NULL)
//          {
//            //replace colon index with null termination 
//            colon_index[0] = 0;
//            //username is from start till our previous null termination
//            char *req_username = &decoded_authorization[0];
//            //the rest is the password
//            char *req_password = &colon_index[1];
//            consoleOut("httpAuthenticateRequest req_username and req_password :"); 
//            // in all cases we get the same data
//            Serial.println(String(server_username)); //admin
//            Serial.println(String(server_password)); // 1234
// //           Serial.println(String(req_username)); // user
// //           Serial.println(String(req_password)); // 1111
//            Serial.println(String(userPwd));          // 1234
// //           Serial.println(String(userPwd));       // 1111                  
//            //ESP_LOGD(TAG, "Username:%s, Password:%s", req_username, req_password);
//            //char server_username[11] = "admin";
//            //check if both username and password match the server's credentials
//             if (strcmp(req_username, "admin") == 0 && strcmp(req_password, pswd) == 0) return true; 
//            
//            Serial.println("no match found");
//          }
//          else
//          {
//            //ESP_LOGD(TAG, "Decoede authorization does not contain password");
//          }
//        }
//        else
//        {
//          //ESP_LOGD(TAG, "Decoding failed");
//        }
//      }
//      else
//      {
//        //ESP_LOGD(TAG, "Authorization value not in correct format");
//      }
//    }
//    else
//    {
//      //ESP_LOGD(TAG, "Cannot retrieve autorization value");
//    }
//  }
//  else
//  {
//    //ESP_LOGD(TAG, "No autorization header or too long");
//  }
//  
//  //ESP_LOGW(TAG, "Authentication Failed");
//  return false;
//}

void confirm() {
toSend="<html><body onload=\"setTimeout(function(){window.location.href='/';}, 3000 );\"><br><br><center><h3>processing<br>your request,<br>please wait</html>";
}

//double round2(double value) {
//   return (int)(value * 100 + 0.5) / 100.0;
//}
//double round1(double value) {
//   return (int)(value * 10 + 0.5) / 10.0;
//}
