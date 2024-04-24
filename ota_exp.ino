//// https ota example
//
//
////the url has to point to a file on ??
//char* url = (char*)arg;
//ESP_LOGI(TAG, "Starting OTA - %s", url);
//        
//esp_http_client_config_t config = {
//  .url = url,
//  .cert_pem = (char *)server_cert_pem_start,
//  .event_handler = _http_event_handler,
//};
//        
//esp_err_t ret = esp_https_ota(&config);
//if (ret == ESP_OK) {
//   indicate_ota_inprogress(false);
//   ESP_LOGE(TAG, "Firmware upgrade SUCCESS");
//   esp_restart();
//} else {
//   indicate_ota_inprogress(false);
//   ESP_LOGE(TAG, "Firmware upgrade failed");
//}
