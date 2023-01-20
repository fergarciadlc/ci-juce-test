module DishTestTool
	class ProtoBuffConnection < TCPSocket
		DEFAULT_TIMEOUT = 50

		def initialize(host, port)
			@connection =  super(host, port)
		end

		def send_result(pb_result, connect_timeout = DEFAULT_TIMEOUT)
			Timeout::timeout(connect_timeout) do
				serialized_result = pb_result.serialize_to_string
				serialized_size = [serialized_result.bytesize].pack('N')

				@connection.print(serialized_size)
				@connection.print(serialized_result)
			end
		end
	end #ProtoBuffConnection
end