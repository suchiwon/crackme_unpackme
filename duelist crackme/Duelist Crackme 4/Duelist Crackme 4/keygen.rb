#Key generator for Duelist Crackme 4
#author afman
#8/8/2011

uname = ARGV[0].to_s
data_array_1 = ['A','1','L','S','K','2','D','J','F','4','H','G','P','3','Q','W',
		'O','5','E','I','R','6','U','T','Y','Z','8','M','X','N','7','C',
		'B','V','9']

data_array_2 = ['S','U','7','C','S','J','K','F','0','9','N','C','S','D','O','9',
		'S','D','F','0','9','S','D','R','L','V','K','7','8','0','9','S',
		'4','N','F']

storage = Array.new(uname.length)

uname.each_byte do |char|
	counter = -1

	#Eliminate invalid characters.
	if char >=0x7a || (char < 0x61 && char >= 0x5a) || char < 0x41 then
		puts "Invalid name entered. Exiting..."
		Kernel.exit
	end

	#Convert to uppercase.
	if char >= 0x61 then
		char -= 0x20
	end
	
	until (char - data_array_1[counter].ord) == 0 do
		counter += 1
	end

	storage.push(data_array_2[counter])
end

puts "Username is: " + uname
puts "Your key is: " + storage.join.to_s
