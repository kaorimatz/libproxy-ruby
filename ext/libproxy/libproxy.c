#include "libproxy.h"

VALUE rb_eLibproxyError;

static void
proxy_factory_free(void *ptr)
{
    px_proxy_factory_free((pxProxyFactory *)ptr);
}

static const rb_data_type_t proxy_factory_data_type = {
    "proxy_factory",
    { NULL, proxy_factory_free, NULL, },
    0, 0, RUBY_TYPED_FREE_IMMEDIATELY
};

static VALUE
rb_proxy_factory_s_allocate(VALUE klass)
{
    return TypedData_Wrap_Struct(klass, &proxy_factory_data_type, 0);
}

/*
 * call-seq:
 *    Libproxy::ProxyFactory.new -> proxy_factory
 *
 * Returns a new Libproxy::ProxyFactory object.
 *
 *    Libproxy::ProxyFactory.new #=> #<Libproxy::ProxyFactory:0x007fde591a6500>
 */
static VALUE
rb_proxy_factory_initialize(VALUE self)
{
    pxProxyFactory *pf = px_proxy_factory_new();
    if (pf == NULL) {
        rb_raise(rb_eLibproxyError, "px_proxy_factory_new");
    }

    DATA_PTR(self) = pf;

    return self;
}

static inline pxProxyFactory *
check_proxy_factory(VALUE self)
{
    return rb_check_typeddata(self, &proxy_factory_data_type);
}

/*
 * call-seq:
 *    proxy_factory.proxies(url) -> [string]
 *
 * Returns a list of proxies to use for <i>url</i> as an array.
 *
 *    Libproxy::ProxyFactory.new.proxies("http://example.com")
 *    #=> ["socks://proxy.example.com:1080", "socks5://proxy.example.com:1080"]
 */
static VALUE
rb_proxy_factory_proxies(VALUE self, VALUE url)
{
    pxProxyFactory *pf = check_proxy_factory(self);
    char **proxies;
    int i;
    VALUE ret;

    StringValue(url);

    proxies = px_proxy_factory_get_proxies(pf, RSTRING_PTR(url));
    if (proxies == NULL) {
        rb_raise(rb_eLibproxyError, "px_proxy_factory_get_proxies: url='%s'", RSTRING_PTR(url));
    }

    ret = rb_ary_new();
    for (i = 0; proxies[i]; i++) {
        rb_ary_push(ret, rb_str_new_cstr(proxies[i]));
        free(proxies[i]);
    }

    free(proxies);

    return ret;
}

void
Init_libproxy(void)
{
    VALUE rb_mLibproxy, rb_cProxyFactory;

    rb_mLibproxy = rb_define_module("Libproxy");
    rb_eLibproxyError = rb_define_class_under(rb_mLibproxy, "Error", rb_eStandardError);

    rb_cProxyFactory = rb_define_class_under(rb_mLibproxy, "ProxyFactory", rb_cObject);
    rb_define_alloc_func(rb_cProxyFactory, rb_proxy_factory_s_allocate);
    rb_define_method(rb_cProxyFactory, "initialize", rb_proxy_factory_initialize, 0);
    rb_define_method(rb_cProxyFactory, "proxies", rb_proxy_factory_proxies, 1);
}
