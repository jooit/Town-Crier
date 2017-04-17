#include <stdio.h>
#include <stdlib.h>
#include <Debug.h>
#include "tls_client.h"
#include "../scrapers/stockticker.h"
#include "Log.h"

#include "scrapers/yahoo_yql_stock.h"
#include "scrapers/yahoo_yql.h"

static int test(const uint8_t * data) {
  YahooYQLStock yahooYQLStock;
  int closing_price;
  err_code err = yahooYQLStock.handler(data, 32 * 2, &closing_price);
  switch (err) {
    case NO_ERROR:
      LL_INFO("closingPrice is %d", closing_price);
      return 0;
    case WEB_ERROR:
      LL_INFO("Returned unexpected WEB_ERROR");
      return -1;
    case INVALID_PARAMS:
      LL_INFO("Returned unexpected INVALID_PARAMS");
      return 0;
    default:
      return -1;
  }
}

int stockticker_self_test() {
  string symbol = "YHOO";
  symbol.append(32 - symbol.length(), 0);
  string unix_epoch = "\x58\x3E\x8C\x80";
  unix_epoch.insert(unix_epoch.begin(), 32 - unix_epoch.length(), '\0');

  string data = symbol + unix_epoch;
  dump_buf("data", (const unsigned char*) data.c_str(), data.size());

  test((const uint8_t *) data.c_str());

  bytes32 ticker("AAPL");
  bytes32 unix_time(1492041600);
  bytes data_(ticker, unix_time);

  dump_buf("data", &data_[0], data_.size());

  test(&data_[0]);

  return 0;
}