#include "UnitTests.hpp"

#include "HttpRequest_test.hpp"
#include "HttpResponse_test.hpp"
#include "Route_test.hpp"
#include "Router_test.hpp"

int main() {    
    
    HttpRequestTests();
    HttpResponseTests();
    RouteTests();
    RouterTests();

    return 0;
}