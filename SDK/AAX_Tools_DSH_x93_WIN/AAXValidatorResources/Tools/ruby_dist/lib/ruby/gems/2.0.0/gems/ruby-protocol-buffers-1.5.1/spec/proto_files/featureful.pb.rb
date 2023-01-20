#!/usr/bin/env ruby
# Generated by the protocol buffer compiler. DO NOT EDIT!

require 'protocol_buffers'

module Featureful
  # forward declarations
  class A < ::ProtocolBuffers::Message; end
  class B < ::ProtocolBuffers::Message; end
  class ABitOfEverything < ::ProtocolBuffers::Message; end
  class C < ::ProtocolBuffers::Message; end
  class D < ::ProtocolBuffers::Message; end
  class E < ::ProtocolBuffers::Message; end
  class F < ::ProtocolBuffers::Message; end

  # enums
  module MainPayloads
    include ::ProtocolBuffers::Enum

    set_fully_qualified_name "featureful.MainPayloads"

    P1 = 2
    P2 = 3
    P3 = 4
  end

  class A < ::ProtocolBuffers::Message
    # forward declarations
    class Sub < ::ProtocolBuffers::Message; end
    class Group1 < ::ProtocolBuffers::Message; end
    class Group2 < ::ProtocolBuffers::Message; end
    class Group3 < ::ProtocolBuffers::Message; end

    set_fully_qualified_name "featureful.A"

    # nested messages
    class Sub < ::ProtocolBuffers::Message
      # forward declarations
      class SubSub < ::ProtocolBuffers::Message; end

      # enums
      module Payloads
        include ::ProtocolBuffers::Enum

        set_fully_qualified_name "featureful.A.Sub.Payloads"

        P1 = 0
        P2 = 1
      end

      set_fully_qualified_name "featureful.A.Sub"

      # nested messages
      class SubSub < ::ProtocolBuffers::Message
        set_fully_qualified_name "featureful.A.Sub.SubSub"

        optional :string, :subsub_payload, 1
      end

      optional :string, :payload, 1
      required ::Featureful::A::Sub::Payloads, :payload_type, 2, :default => ::Featureful::A::Sub::Payloads::P1
      optional ::Featureful::A::Sub::SubSub, :subsub1, 3
    end

    class Group1 < ::ProtocolBuffers::Message
      # forward declarations
      class Subgroup < ::ProtocolBuffers::Message; end

      set_fully_qualified_name "featureful.A.Group1"

      # nested messages
      class Subgroup < ::ProtocolBuffers::Message
        set_fully_qualified_name "featureful.A.Group1.Subgroup"

        required :int32, :i1, 1
      end

      required :int32, :i1, 1
      repeated ::Featureful::A::Group1::Subgroup, :subgroup, 2, :group => true
    end

    class Group2 < ::ProtocolBuffers::Message
      # forward declarations
      class Subgroup < ::ProtocolBuffers::Message; end

      set_fully_qualified_name "featureful.A.Group2"

      # nested messages
      class Subgroup < ::ProtocolBuffers::Message
        set_fully_qualified_name "featureful.A.Group2.Subgroup"

        required :int32, :i1, 1
      end

      required :int32, :i1, 1
      repeated ::Featureful::A::Group2::Subgroup, :subgroup, 2, :group => true
    end

    class Group3 < ::ProtocolBuffers::Message
      # forward declarations
      class Subgroup < ::ProtocolBuffers::Message; end

      set_fully_qualified_name "featureful.A.Group3"

      # nested messages
      class Subgroup < ::ProtocolBuffers::Message
        set_fully_qualified_name "featureful.A.Group3.Subgroup"

        required :int32, :i1, 1
      end

      required :int32, :i1, 1
      repeated ::Featureful::A::Group3::Subgroup, :subgroup, 2, :group => true
    end

    repeated :int32, :i1, 1
    optional :int32, :i2, 2
    required :int32, :i3, 3
    repeated ::Featureful::A::Sub, :sub1, 4
    optional ::Featureful::A::Sub, :sub2, 5
    required ::Featureful::A::Sub, :sub3, 6
    repeated ::Featureful::A::Group1, :group1, 7, :group => true
    optional ::Featureful::A::Group2, :group2, 8, :group => true
    required ::Featureful::A::Group3, :group3, 9, :group => true
  end

  class B < ::ProtocolBuffers::Message
    set_fully_qualified_name "featureful.B"

    repeated ::Featureful::A, :a, 1
  end

  class ABitOfEverything < ::ProtocolBuffers::Message
    set_fully_qualified_name "featureful.ABitOfEverything"

    optional :double, :double_field, 1
    optional :float, :float_field, 2
    optional :int32, :int32_field, 3
    optional :int64, :int64_field, 4, :default => 15
    optional :uint32, :uint32_field, 5
    optional :uint64, :uint64_field, 6
    optional :sint32, :sint32_field, 7
    optional :sint64, :sint64_field, 8
    optional :fixed32, :fixed32_field, 9
    optional :fixed64, :fixed64_field, 10
    optional :sfixed32, :sfixed32_field, 11
    optional :sfixed64, :sfixed64_field, 12
    optional :bool, :bool_field, 13, :default => false
    optional :string, :string_field, 14, :default => "zomgkittenz"
    optional :bytes, :bytes_field, 15
  end

  class C < ::ProtocolBuffers::Message
    set_fully_qualified_name "featureful.C"

    optional ::Featureful::D, :d, 1
    repeated ::Featureful::E, :e, 2
  end

  class D < ::ProtocolBuffers::Message
    set_fully_qualified_name "featureful.D"

    repeated ::Featureful::F, :f, 1
    optional ::Featureful::F, :f2, 2
  end

  class E < ::ProtocolBuffers::Message
    set_fully_qualified_name "featureful.E"

    optional ::Featureful::D, :d, 1
  end

  class F < ::ProtocolBuffers::Message
    set_fully_qualified_name "featureful.F"

    optional :string, :s, 1
  end

end