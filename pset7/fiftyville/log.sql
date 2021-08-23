-- Keep a log of any SQL queries you execute as you solve the mystery.

SELECT description FROM crime_scene_reports WHERE month=7 AND day=28 AND street="Chamberlin Street";
SELECT name, transcript FROM interviews WHERE month=7 AND day=28 AND transcript LIKE "%courthouse%";
SELECT license_plate FROM people WHERE name="Ruth";
SELECT hour, minute FROM courthouse_security_logs WHERE license_plate="HZB4129";
-- So Ruth was just walking by the courthouse then? That is the only way they would have witnessed this since they did not enter or exit the parking lot. At least not in their own car.
SELECT caller, receiver FROM phone_calls WHERE month=7 AND day=28 AND duration<60;
-- More than one call per Raymond's description. Have to find the call that happened as someone was leaving the courthouse. But time's do not include hour or minute.
-- Must find a person that exited the courthouse and had a phone call that lasted less than a minute as the caller (not receiver) and both of those events occured on July 28, 2020.
SELECT caller.license_plate, caller.name, caller.phone_number, receiver.license_plate, receiver.name, receiver.phone_number
FROM courthouse_security_logs logs
JOIN people caller ON (logs.license_plate = caller.license_plate)
JOIN phone_calls call ON (call.caller = caller.phone_number)
JOIN people receiver ON (call.receiver = receiver.phone_number)
WHERE logs.month=7 AND logs.day=28 AND logs.activity="exit" AND call.month=7 AND call.day=28 AND call.duration<60;
/*
NEW SUSPECTS (THIEF):                   NEW SUSPECTS (ACCOMPLICE):
94KL13X | Ernest  | (367) 555-5533      4V16VO0 | Berthold | (375) 555-8161
G412CB7 | Roger   | (130) 555-0289      52R0Y8U | Jack     | (996) 555-8899
322W7JE | Russell | (770) 555-1861      GW362R6 | Philip   | (725) 555-3243
0NTHK55 | Evelyn  | (499) 555-9472      N/A     | Melissa  | (717) 555-1342
0NTHK55 | Evelyn  | (499) 555-9472      O268ZZ0 | Larry    | (892) 555-8872
1106N58 | Madison | (286) 555-6063      Q13SVG6 | James    | (676) 555-6554
*/
-- FIND AN ATM TRANSACTION THAT IS A WITHDRAWEL ON FIFER STREET ON 07/28. Find bank account with the same account number withdrawel was from. Find the person with that bank account person_id. If that person is one of the suspected accomplices, we have narrowed down our leads.
SELECT person.name
FROM atm_transactions atm
JOIN bank_accounts account ON (account.account_number = atm.account_number)
JOIN people person ON (person.id = account.person_id)
WHERE atm.atm_location = "Fifer Street" AND atm.transaction_type = "withdraw" AND atm.month = 7 AND atm.day = 28;
/*
I misunderstood. The THEIF withdrew money. The accomplice already had money and purchased tickets. Regardless, we can still narrow down the list.
NEW SUSPECTS (THIEF):                   NEW SUSPECTS (ACCOMPLICE):
94KL13X | Ernest  | (367) 555-5533      4V16VO0 | Berthold | (375) 555-8161
322W7JE | Russell | (770) 555-1861      GW362R6 | Philip   | (725) 555-3243
1106N58 | Madison | (286) 555-6063      Q13SVG6 | James    | (676) 555-6554
*/
SELECT flight.id, person.name, passenger.seat, origin.city, destination.city, flight.month, flight.day
FROM passengers passenger
JOIN people person ON (person.passport_number = passenger.passport_number)
JOIN flights flight ON (flight.id = passenger.flight_id)
JOIN airports origin ON (origin.id = flight.origin_airport_id)
JOIN airports destination ON (destination.id = flight.destination_airport_id)
WHERE (person.name = "Ernest" OR person.name = "Russell" OR person.name = "Madison" OR person.name = "Berthold" OR person.name = "Philip" OR person.name = "James") AND flight.month = 7
ORDER BY flight.day ASC;
/*
New suspects based on flight and seat. (Suspected accomplice and suspected thief on same flight or going to same destination)
NEW SUSPECTS (THIEF):                   NEW SUSPECTS (ACCOMPLICE):
1106N58 | Madison | (286) 555-6063      Q13SVG6 | James    | (676) 555-6554         <-- ONLY TWO ASSOCIATED SUSPECTS THAT WERE GOING TO SAME DESTINATION
*/
-- Doesn't seem to work. Going back and testing out previous set of suspects.
-- Seems to be first set which is Ernest, Berthold. Ernest is traveling to London.