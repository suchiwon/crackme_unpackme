limit = (2**(ARGV[0].to_i))-1 #The upper limit to count to

#Data array of characters.
data_array = [0x16,0x49,0x5e,0x15,0x27,0x26,0x21,0x25,0x1d,0x59,
           0x53,0x37,0x31,0x48,0x5d,0x0c,0x61,0x52,0x4d]

#For every possible combination from 0 to 2^n...
0.upto(limit) do |i| 

  current_string = "%018b" % i  #convert to 18 character padded binary
  counter = 0
  total_sum = 0

  if i == 78758 then
    puts 
  end

  print "Current String: " + current_string
  print " Current Number: " + i.to_s

  until data_array[counter] == 0x4d do
    
    resource_id = data_array[counter]
    
    counter+=1

    if current_string[14].to_i == 1 then
      total_sum = resource_id*
      data_array[counter]*counter + total_sum
    end
    
    resource_id = data_array[counter]
    counter += 1

    if current_string[16].to_i == 1 then
      total_sum = resource_id*
      data_array[counter]*counter + total_sum
    end
    
    resource_id = data_array[counter]
    counter += 1

    if current_string[15].to_i == 1 then
      total_sum = resource_id*
      data_array[counter]*counter + total_sum
    end
    
    resource_id = data_array[counter]
    counter += 1

    if current_string[8].to_i == 1 then
      total_sum = resource_id*
      data_array[counter]*counter + total_sum
    end
    
    resource_id = data_array[counter]
    counter += 1
    
    if current_string[1].to_i == 1 then
      total_sum = resource_id*
      data_array[counter]*counter + total_sum
    end
    
    resource_id = data_array[counter]
    counter += 1

    if current_string[12].to_i == 1 then
      total_sum = resource_id*
      data_array[counter]*counter + total_sum
    end 
    
    resource_id = data_array[counter]
    counter += 1

    if current_string[11].to_i == 1 then
      total_sum = resource_id*
      data_array[counter]*counter + total_sum 
    end
    
    resource_id = data_array[counter]
    counter += 1

    if current_string[13].to_i == 1 then
      total_sum = resource_id*
      data_array[counter]*counter + total_sum
    end
    
    resource_id = data_array[counter]
    counter += 1

    if current_string[0].to_i == 1 then
      total_sum = resource_id*
      data_array[counter]*counter + total_sum 
    end 
    
    resource_id = data_array[counter]
    counter += 1

    if current_string[10].to_i == 1 then
      total_sum = resource_id*
      data_array[counter]*counter + total_sum
    end
    
    resource_id = data_array[counter]
    counter += 1

    if current_string[9].to_i == 1 then
      total_sum = resource_id*
      data_array[counter]*counter + total_sum
    end
    
    resource_id = data_array[counter]
    counter += 1

    if current_string[7].to_i == 1 then
      total_sum = resource_id*
      data_array[counter]*counter + total_sum 
    end
    
    resource_id = data_array[counter]
    counter += 1

    if current_string[6].to_i == 1 then
      total_sum = resource_id*
      data_array[counter]*counter + total_sum
    end
    
    resource_id = data_array[counter]
    counter += 1

    if current_string[5].to_i == 1 then
      total_sum = resource_id*
      data_array[counter]*counter + total_sum 
    end 
    
    resource_id = data_array[counter]
    counter += 1

    if current_string[4].to_i == 1 then
      total_sum = resource_id*
      data_array[counter]*counter + total_sum 
    end 
    
    resource_id = data_array[counter]
    counter += 1

    if current_string[3].to_i == 1 then
      total_sum = resource_id*
      data_array[counter]*counter + total_sum 
    end
    
    resource_id = data_array[counter]
    counter += 1

    if current_string[17].to_i == 1 then
      total_sum = resource_id*
      data_array[counter]*counter + total_sum 
    end
    
    resource_id = data_array[counter]
    counter += 1
    
    if current_string[2].to_i == 1 then
      total_sum = resource_id*
      data_array[counter]*counter + total_sum
    end

  end

  puts " Total Sum: " + total_sum.to_s(16)

  #If the total sum is 0x328FE then we have found the
  if total_sum.to_i == 0x328fe then
    puts "Correct sequence is: " + current_string
    break
  end

end
