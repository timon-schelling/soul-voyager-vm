#!/usr/bin/env kscript

import java.io.*

fun UInt.toUBytes() = listOf((this shr 24).toUByte(), (this shr 16).toUByte(), (this shr 8).toUByte(), this.toUByte())

val output = mutableListOf<UByte>()

while(true) {
    val line = readLine()
    val cmd = line?.substringBefore(" ")
    val opt = when (cmd) {
        "HLT" -> 0

        "MOV_REG_TO_REG" -> 1
        "MOV_ADDRESS_TO_REG" -> 2
        "MOV_REGADDRESS_TO_REG" -> 3
        "MOV_REG_TO_ADDRESS" -> 4
        "MOV_REG_TO_REGADDRESS" -> 5
        "MOV_NUMBER_TO_REG" -> 6
        "MOV_NUMBER_TO_ADDRESS" -> 7
        "MOV_NUMBER_TO_REGADDRESS" -> 8

        "ADD_REG_TO_REG" -> 9
        "ADD_REGADDRESS_TO_REG" -> 10
        "ADD_ADDRESS_TO_REG" -> 11
        "ADD_NUMBER_TO_REG" -> 12

        "SUB_REG_FROM_REG" -> 13
        "SUB_REGADDRESS_FROM_REG" -> 14
        "SUB_ADDRESS_FROM_REG" -> 15
        "SUB_NUMBER_FROM_REG" -> 16

        "INC_REG" -> 17

        "DEC_REG" -> 18

        "CMP_REG_WITH_REG" -> 19
        "CMP_REGADDRESS_WITH_REG" -> 20
        "CMP_ADDRESS_WITH_REG" -> 21
        "CMP_NUMBER_WITH_REG" -> 23

        "JMP_REGADDRESS" -> 24
        "JMP_ADDRESS" -> 25

        "JC_REGADDRESS" -> 26
        "JC_ADDRESS" -> 27

        "JNC_REGADDRESS" -> 28
        "JNC_ADDRESS" -> 29

        "JZ_REGADDRESS" -> 30
        "JZ_ADDRESS" -> 31

        "JNZ_REGADDRESS" -> 32
        "JNZ_ADDRESS" -> 33

        "JA_REGADDRESS" -> 34
        "JA_ADDRESS" -> 35

        "JNA_REGADDRESS" -> 36
        "JNA_ADDRESS" -> 37

        "PUSH_REG" -> 38
        "PUSH_REGADDRESS" -> 39
        "PUSH_ADDRESS" -> 40
        "PUSH_NUMBER" -> 41

        "POP_REG" -> 42
        "POP_REGADDRESS" -> 43
        "POP_ADDRESS" -> 44

        "CALL_REGADDRESS" -> 45
        "CALL_ADDRESS" -> 46

        "RET" -> 47

        "MUL_REG" -> 48
        "MUL_REGADDRESS" -> 49
        "MUL_ADDRESS" -> 50
        "MUL_NUMBER" -> 51

        "DIV_REG" -> 52
        "DIV_REGADDRESS" -> 53
        "DIV_ADDRESS" -> 54
        "DIV_NUMBER" -> 55

        "AND_REG_WITH_REG" -> 56
        "AND_REGADDRESS_WITH_REG" -> 57
        "AND_ADDRESS_WITH_REG" -> 58
        "AND_NUMBER_WITH_REG" -> 59

        "OR_REG_WITH_REG" -> 60
        "OR_REGADDRESS_WITH_REG" -> 61
        "OR_ADDRESS_WITH_REG" -> 62
        "OR_NUMBER_WITH_REG" -> 63

        "XOR_REG_WITH_REG" -> 64
        "XOR_REGADDRESS_WITH_REG" -> 65
        "XOR_ADDRESS_WITH_REG" -> 66
        "XOR_NUMBER_WITH_REG" -> 67

        "NOT_REG" -> 68

        "SHL_REG_WITH_REG" -> 69
        "SHL_REGADDRESS_WITH_REG" -> 70
        "SHL_ADDRESS_WITH_REG" -> 71
        "SHL_NUMBER_WITH_REG" -> 72

        "SHR_REG_WITH_REG" -> 73
        "SHR_REGADDRESS_WITH_REG" -> 74
        "SHR_ADDRESS_WITH_REG" -> 75
        "SHR_NUMBER_WITH_REG" -> 76

        else -> break
    }
    output.add(opt.toUByte())

    if(!line.contains(" ")) continue
    val args = line?.substringAfter(" ")
    if(args.isNullOrBlank()) continue
    val arg1 = args?.substringBefore(",")
    if(arg1.isNullOrBlank()) continue
    output.addAll(arg1.toUInt().toUBytes())
    if(!args.contains(",")) continue
    val arg2 = args?.substringAfterLast(",")
    if(arg2.isNullOrBlank()) continue
    output.addAll(arg2.toUInt().toUBytes())
}

File("prgm.bin").writeBytes(output.toUByteArray().toByteArray())
