require 'spec_helper'

RSpec.describe Libproxy::ProxyFactory do
  let(:proxy_factory) { described_class.new }

  describe '#proxies' do
    it 'returns an array of proxies' do
      expect(proxy_factory.proxies('http://example.com/')).to eq(['direct://'])
    end
  end
end
