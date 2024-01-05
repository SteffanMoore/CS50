-- Keep a log of any SQL queries you execute as you solve the mystery.

-- See the structure of the database using .schema

-- Looking at the crime scene report for this outrageous theft
SELECT * FROM crime_scene_reports
WHERE street = 'Humphrey Street'
AND day = 28
AND month = 7;

-- Three eyewitnesses interviewed on 28/7/2021 all mentioned the bakery. Theft took place at 10:15am.

-- Accessing interviews
SELECT * FROM interviews
WHERE day = 28
AND month = 7;

-- Ruth ---- Within 10 mins of theft thief got into car in bakery car park and left.
-- Eugene -- Recognises the thief but doesn't know the name - before Eugene went to bakery she saw the thief at the Leggett st.
--           ATM.
-- Raymond - Thief left bakery on the phone (less than a minute)- earliest flight on 29/7/2021 which someone else is buying for
--           them.


-- Checking bakery CCTV
SELECT * FROM bakery_security_logs
WHERE month = 7
AND day = 28
AND hour > 9
AND hour < 11
AND minute < 26;

-- Checking phone calls
SELECT * FROM phone_calls
WHERE duration < 60
AND month = 7
AND day = 28;

-- Checking ATM
SELECT * FROM atm_transactions
WHERE atm_location = 'Leggett Street'
AND day = 28
AND month = 7;

-- Checking bank accounts
SELECT * FROM bank_accounts
WHERE account_number
IN (SELECT account_number FROM atm_transactions
    WHERE atm_location = 'Leggett Street'
    AND day = 28
    AND month = 7);

-- Checking airports
SELECT * FROM airports
WHERE city = 'Fiftyville';

-- Airport ID is 8 for Fiftyville

-- Checking flights
SELECT * FROM flights
WHERE origin_airport_id = 8
AND day = 29
ORDER BY hour ASC;

-- Flight ID is 36 leaves at 8:20 29/7/2021 and travels to an airport of ID 4 - LaGuardia Airport (New York City)

-- Checking flight passengers
SELECT * FROM passengers
WHERE flight_id = 36;

-- Check for common persons
SELECT * FROM people
WHERE phone_number
IN (SELECT caller FROM phone_calls
    WHERE duration < 60
    AND month = 7
    AND day = 28)
AND passport_number
IN (SELECT passport_number FROM passengers
    WHERE flight_id = 36)
AND license_plate
IN (SELECT license_plate FROM bakery_security_logs
    WHERE month = 7
    AND day = 28
    AND hour > 9
    AND hour < 11
    AND minute < 26)
AND id
IN (SELECT person_id FROM bank_accounts
    WHERE account_number
    IN (SELECT account_number FROM atm_transactions
        WHERE atm_location = 'Leggett Street'
        AND day = 28
        AND month = 7));

-- G'day Bruce we've found you!
-- Now to find the accomplice

SELECT * FROM people
WHERE phone_number
IN (SELECT receiver FROM phone_calls
    WHERE caller
    IN (SELECT phone_number FROM people
        WHERE name = 'Bruce')
    AND month = 7
    AND day = 28
    AND duration < 60);

-- It's Robin!
