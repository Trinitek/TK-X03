--
-- TK3081 Assembler
-- Written by Blake Burgess
-- March 2014
--

-- /// CONSTANTS ///
local VERSION = 1.1
local ERROR_BAD_ARGUMENTS = 160
local ERROR_OPEN_FAILED = 110

-- // KEYWORD DEFINITIONS //

local keywordDictionary = {
	-- instruction set
	["NOP"]		= 0x00,
	["STC"]		= 0x01,
	["CLC"]		= 0x02,
	["HLT"]		= 0x08,
	["INA"]		= 0x03,
	["INI"]		= 0x04,
	["OUTAB"]	= 0x05,
	["OUTAI"]	= 0x06,
	["OUTII"]	= 0x07,
	["MOVAMX"]	= 0x10,
	["MOVAMH"]	= 0x11,
	["MOVAML"]	= 0x12,
	["MOVASP"]	= 0x13,
	["MOVAF"]	= 0x14,
	["MOVAB"]	= 0x15,
	["MOVMXA"]	= 0x16,
	["MOVMHA"]	= 0x17,
	["MOVMLA"]	= 0x18,
	["MOVSPA"]	= 0x19,
	["MOVBA"]	= 0x1A,
	["MOVI"]	= 0x1B,
	["XCHG"]	= 0x1C,
	["AND"]		= 0x20,
	["NAND"]	= 0x21,
	["OR"]		= 0x22,
	["NOR"]		= 0x23,
	["XNOR"]	= 0x24,
	["NOT"]		= 0x25,
	["SHL"]		= 0x26,
	["SHR"]		= 0x27,
	["ROL"]		= 0x28,
	["ROR"]		= 0x29,
	["CMP"]		= 0x2A,
	["XOR"]		= 0x2B,
	["JMPMX"]	= 0x30,
	["JMPI"]	= 0x31,
	["JEMX"]	= 0x32,
	["JEI"]		= 0x33,
	["JNEMX"]	= 0x34,
	["JNEI"]	= 0x35,
	["JGMX"]	= 0x36,
	["JGI"]		= 0x37,
	["JLMX"]	= 0x38,
	["JLI"]		= 0x39,
	["JCMX"]	= 0x3A,
	["JCI"]		= 0x3B,
	["JNCMX"]	= 0x3C,
	["JNCI"]	= 0x3D,
	["CALLMX"]	= 0x3E,
	["CALLI"]	= 0x3F,
	["RET"]		= 0x40,
	["PUSHA"]	= 0x41,
	["PUSHB"]	= 0x42,
	["PUSHMX"]	= 0x43,
	["PUSHMH"]	= 0x44,
	["PUSHML"]	= 0x45,
	["PUSHF"]	= 0x46,
	["PUSHI"]	= 0x47,
	["POPA"]	= 0x48,
	["POPB"]	= 0x49,
	["POPMX"]	= 0x4A,
	["POPMH"]	= 0x4B,
	["POPML"]	= 0x4C,
	["ADDA"]	= 0x50,
	["ADDI"]	= 0x51,
	["SUBA"]	= 0x52,
	["SUBI"]	= 0x53,
	["INC"]		= 0x54,
	["DEC"]		= 0x55,
	
	-- special keywords
	["DB"]		= 0
}

local immdataDictionary = {
	-- instruction set
	["INI"]		= 1,
	["OUTAI"]	= 1,
	["OUTII"]	= 2,
	["MOVI"]	= 1,
	["JMPI"]	= 2,
	["JEI"]		= 2,
	["JNEI"]	= 2,
	["JGI"]		= 2,
	["JLI"]		= 2,
	["JCI"]		= 2,
	["JNCI"]	= 2,
	["CALLI"]	= 2,
	["PUSHI"]	= 1,
	["ADDI"]	= 1,
	["SUBI"]	= 1,
	
	-- special keywords
	["DB"]		= 1
}

-- // GLOBAL DEFINITIONS //

local hexDigits = {
	["0"]		= 0,
	["1"]		= 1,
	["2"]		= 2,
	["3"]		= 3,
	["4"]		= 4,
	["5"]		= 5,
	["6"]		= 6,
	["7"]		= 7,
	["8"]		= 8,
	["9"]		= 9,
	["A"]		= 10,
	["B"]		= 11,
	["C"]		= 12,
	["D"]		= 13,
	["E"]		= 14,
	["F"]		= 15
}

local dataToOutput_index = 1
local dataToOutput = {}

-- // FUNCTIONS //

function stringToHex(str)
	local outputHex = 0
	
	local secondDigit = string.char(string.byte(str, 1))
	local firstDigit = string.char(string.byte(str, 2))
	
	for hexChar,hexNumber in pairs(hexDigits) do
		if hexChar == secondDigit then
			-- shift second digit to leftmost place
			outputHex = outputHex + hexNumber * 0x10
		end
		
		if hexChar == firstDigit then
			-- first digit stays in rightmost place
			outputHex = outputHex + hexNumber
		end
	end
	
	return outputHex
end

function exportToFile()
	local destFile = io.open(destFile_name, "w+b")
	
	-- can't open destination file? exit with an error
	if destFile == nil then
		print("ERROR: cannot open destination file.")
		os.exit(ERROR_OPEN_FAILED)
	end
	
	--file writes are done as text; bind the hex codes to their equivalent chars
	for i=1, #dataToOutput do
		--print("OUTPUT BYTE #" .. i)
		destFile:write(string.char(dataToOutput[i]))
	end
	
	print(#dataToOutput .. " bytes written to file")
	
	io.close(destFile)
end

function checkForComments(keyword)
	-- extract first character and check for semicolon
	if string.char(string.byte(keyword, 1)) == ';' then
		return true
	else
		return false
	end
end

-- // CODE //

print()
print("TK3081 Assembler  //  Version " .. VERSION .. "  //  Copyright 2014 Blake Burgess")
print("Usage: 3081ASM  <source>  [destination]")
print()

sourceFile_name = arg[1]
destFile_name = arg[2]

-- check for missing arguments
if sourceFile_name == nil then
	print("ERROR: No source file specified.")
	os.exit(ERROR_BAD_ARGUMENTS)
end

-- dest file unspecified? cut off sourceFile's extension and use that name instead
if destFile_name == nil then
	-- get rid of everything after the first '.'
	for sourceFile_noExtension in string.gmatch(sourceFile_name, '%P+') do
		destFile_name = sourceFile_noExtension .. ".bin"
		break
	end
end

-- display source and destination filename for verification
print(sourceFile_name .. " -> " .. destFile_name .. "\n")

sourceFile = io.open(sourceFile_name)

-- can't open source file? exit with an error
if sourceFile == nil then
	print("ERROR: cannot open source file.")
	os.exit(ERROR_OPEN_FAILED)
end

local lineNumber = 1

while true do
	local opcodeList = {}
	local validKeywordFound = false

	lineToParse = (sourceFile:read("*l"))		-- read the entire next line
	if lineToParse == nil then break end		-- EOF? then break
	lineToParse = string.upper(lineToParse)		-- uppercase the line
	
	-- extract keywords from line and store in an array
	local keywordList = {}
	local keywordList_index = 1
	for extractedWord in string.gmatch(lineToParse, '%S+') do
		keywordList[keywordList_index] = extractedWord
		keywordList_index = keywordList_index + 1
	end
	
	-- if this line is blank, then process the next line
	if keywordList[1] == nil then
		goto continue_nextLine
	end
	
	-- if the first keyword is a comment, then process the next line
	if checkForComments(keywordList[1]) == true then
		goto continue_nextLine
	end
	
	-- display info about current line for debugging purposes
	io.write(lineNumber .. ": " .. lineToParse)
	io.write("\n (" .. #keywordList .. ") = ")
	
	-- compare the found keywords against the instruction set
	for mnemonic,opcode in pairs(keywordDictionary) do
		
		-- check for valid keywords
		if keywordList[1] == mnemonic then
			
			-- assign opcode if mnemonic found, treat "DB" token special
			if mnemonic ~= "DB" then
				opcodeList[1] = opcode
				validKeywordFound = true
				io.write(string.format("%x ", opcode))
			elseif mnemonic == "DB" then
				validKeywordFound = true
			end
			
			-- process any immediate data
			for mnem_extra,immdata_count in pairs(immdataDictionary) do
				-- does the mnemonic need extra immediate data?
				if ((mnem_extra == keywordList[1]) and (mnem_extra ~= "DB")) then
					for i=2, immdata_count+1 do
						if keywordList[i] == nil then
							io.write("ERROR: expected immediate data\n")
							goto abort
						else
							opcodeList[i] = stringToHex(keywordList[i])
							io.write(string.format("%x ", opcodeList[i]))
						end
					end
				
				elseif ((mnem_extra == keywordList[1]) and (mnem_extra == "DB")) then
					
					-- no immediate data? if not, show an error and exit
					if keywordList[2] == nil then
						io.write("ERROR: expected immediate data\n")
						goto abort
					end
					
					-- offset keywordList index by 1 to point to immData (disregard "DB" token)
					for i=1, #keywordList - 1 do
						
						-- jump to next line if any comments are found
						if checkForComments(keywordList[i + 1]) == true then
							print()
							goto continue_nextLine
						end
						
						opcodeList[i] = stringToHex(keywordList[i + 1])
						io.write(string.format("%x ", opcodeList[i]))
					end
				end
			end
			
		end
	end
	
	-- was a valid keyword found? if not, show an error and exit
	if validKeywordFound == false then
		io.write("ERROR: invalid keyword\n")
		goto abort
	end
	
	-- write processed data for this line to the table of data to write to file
	for i=1, #opcodeList do
		dataToOutput[dataToOutput_index] = opcodeList[i]
		dataToOutput_index = dataToOutput_index + 1
	end
	
	print()
	
	::continue_nextLine::
	lineNumber = lineNumber + 1
end

print()

-- show how many seconds taken to run the program
print("Assembled in " .. os.clock() .. " seconds")

-- output the compiled data from the table into the output file
exportToFile()

::abort::

io.close(sourceFile)
