-- Keep a log of any SQL queries you execute as you solve the mystery.
SELECT *
FROM crime_scene_reports
WHERE street = 'Humphrey Street'
AND month = 7
AND day = 28;
-- Found bakery mention

-- look for bakery
SELECT *
FROM interviews
WHERE transcript
LIKE '%bakery%'
AND month = 7
AND day = 28;
-- usefull informations:
-- Ruth: within ten minutes of the theft, the thief get into a car in the bakery parking lot and drive away. See the securage footage
-- Eugene: recognized the thief. Saw money withdraw on Leggett Street
-- Raymond: saw the thief leaving the bakery and recieving a call from someone who talked to them for less than a minute. In the call,  the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow (29/7). The thief then asked the person on the other end of the phone to purchase the flight ticket

-- look for money withdraw acording to Eugene's interview
SELECT *
FROM atm_transactions
WHERE month = 7
AND day = 28
AND atm_location = 'Leggett Street'
AND transaction_type = 'withdraw';
-- look for peoples information acording to their account number
SELECT p.id, p.name, p.phone_number, p.passport_number, p.license_plate, a.amount
FROM bank_accounts b
JOIN atm_transactions a ON a.account_number = b.account_number
JOIN people p ON p.id = b.person_id
WHERE a.month = 7
AND a.day = 28
AND a.atm_location = 'Leggett Street'
AND a.transaction_type = 'withdraw';
-- this persons are on the suspect list

-- look for fone calls according to Raymond's interview
SELECT f.id, f.caller, f.receiver, f.duration
FROM phone_calls f
WHERE f.caller
IN
-- list of suspects
(SELECT p.phone_number
FROM bank_accounts b
JOIN atm_transactions a ON a.account_number = b.account_number
JOIN people p ON p.id = b.person_id
WHERE a.month = 7
AND a.day = 28
AND a.atm_location = 'Leggett Street'
AND a.transaction_type = 'withdraw')
AND f.duration < 60; -- assuming that the durations are seconds
-- the receivers are suspects of accomplice

-- according to Raymond's interview the thief mention a flight out of the city (first fligth). lets take a look on the airports and flights
SELECT *
FROM airports
WHERE city = "Fiftyville";
-- this is Fiftyville airport information

-- look for fligths on 29/7 from Fiftyville
SELECT *
FROM flights
WHERE origin_airport_id
IN
(SELECT id
FROM airports
WHERE city = "Fiftyville")
AND month = 7
AND day = 29
-- first fligth
ORDER BY hour, minute
LIMIT 1;

-- look for airport information with the id find above
SELECT *
FROM airports
WHERE id
IN
(SELECT destination_airport_id
FROM flights
WHERE origin_airport_id
IN
(SELECT id
FROM airports
WHERE city = "Fiftyville")
AND month = 7
AND day = 29
ORDER BY hour, minute
LIMIT 1);
-- this may be the place to were the thief escaped to: New York City

-- look for peoples on the flight
SELECT people.name, people.phone_number
FROM passengers
JOIN flights ON flights.id = passengers.flight_id
JOIN people ON people.passport_number = passengers.passport_number
WHERE flights.id = 36 -- the New York City flight
AND people.name
IN -- suspect list
(SELECT people.name
FROM bank_accounts
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
JOIN people ON people.id = bank_accounts.person_id
WHERE atm_transactions.month = 7
AND atm_transactions.day = 28
AND atm_transactions.atm_location = 'Leggett Street'
AND atm_transactions.transaction_type = 'withdraw');
-- this are the reduced suspect list:
-- Bruce
-- Taylor
-- Kenny
-- Luca


-- call caler
SELECT name, phone_calls.duration
FROM people
JOIN phone_calls
ON people.phone_number = phone_calls.caller
WHERE phone_calls.year = 2021
AND phone_calls.month = 7
AND phone_calls.day = 28
AND phone_calls.duration <= 60;
-- call receiver
SELECT name, phone_calls.duration
FROM people
JOIN phone_calls
ON people.phone_number = phone_calls.receiver
WHERE phone_calls.year = 2021
AND phone_calls.month = 7
AND phone_calls.day = 28
AND phone_calls.duration <= 60;

-- Ruth: The thief drove away in a car from the bakery, within 10 minutes from the theft. SO, checking the license plates of cars within that time frame. Then, checking out the names of those cars' owners. They could be suspects.
SELECT name, bakery_security_logs.hour, bakery_security_logs.minute
FROM people
JOIN bakery_security_logs
ON people.license_plate = bakery_security_logs.license_plate
WHERE bakery_security_logs.year = 2021
AND bakery_security_logs.month = 7
AND bakery_security_logs.day = 28
AND bakery_security_logs.activity = 'exit'
AND bakery_security_logs.hour = 10
AND bakery_security_logs.minute >= 15
AND bakery_security_logs.minute <= 25
ORDER BY bakery_security_logs.minute;