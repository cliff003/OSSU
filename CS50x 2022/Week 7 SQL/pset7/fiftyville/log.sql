-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Investigate Crime Scene
SELECT description
  FROM crime_scene_reports
 WHERE year = 2021
   AND month = 7
   AND day = 28
   AND street = "Humphrey Street";

/*
Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery. |
| Littering took place at 16:36. No known witnesses.                                                                                                                                                                     |
*/

-- Investigate interviews
SELECT name, transcript
  FROM interviews
 WHERE year = 2021
   AND month = 7
   AND day = 28;

/*
| Jose    | “Ah,” said he, “I forgot that I had not seen you for some weeks. It is a little souvenir from the King of Bohemia in return for my assistance in the case of the Irene Adler papers.”                                                                                                                               |
| Eugene  | “I suppose,” said Holmes, “that when Mr. Windibank came back from France he was very annoyed at your having gone to the ball.”                                                                                                                                                                                      |
| Barbara | “You had my note?” he asked with a deep harsh voice and a strongly marked German accent. “I told you that I would call.” He looked from one to the other of us, as if uncertain which to address.                                                                                                                   |
| Ruth    | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
| Eugene  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
| Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |
| Lily    | Our neighboring courthouse has a very annoying rooster that crows loudly at 6am every day. My sons Robert and Patrick took the rooster to a city far, far away, so it may never bother us again. My sons have successfully arrived in Paris.                                                                        |
| Emma    | I'm the bakery owner, and someone came in, suspiciously whispering into a phone for about half an hour. They never bought anything.
*/

-- Investigate ATM on Leggett Street to identify theft, create a suspect list
SELECT p.name, p.id, a.account_number, a.amount, a.transaction_type
  FROM atm_transactions AS a
    LEFT JOIN bank_accounts AS b
    USING (account_number)
    LEFT JOIN people AS p
       ON b.person_id = p.id
 WHERE a.year = 2021
   AND a.month = 7
   AND a.day = 28
   AND a.atm_location = "Leggett Street"
   ORDER BY p.name;

/*
|  name   |   id   | account_number | amount | transaction_type |
| Benista | 438727 | 81061156       | 30     | withdraw         |
| Brooke  | 458378 | 16153065       | 80     | withdraw         |
| Bruce   | 686048 | 49610011       | 50     | withdraw         |
| Diana   | 514354 | 26013199       | 35     | withdraw         |
| Iman    | 396669 | 25506511       | 20     | withdraw         |
| Kaelyn  | 948985 | 86363979       | 10     | deposit          |
| Kenny   | 395717 | 28296815       | 20     | withdraw         |
| Luca    | 467400 | 28500762       | 48     | withdraw         |
| Taylor  | 449774 | 76054385       | 60     | withdraw
*/

-- Cross Validate with bakery security footage
WITH t1 AS
(SELECT p.name, p.license_plate
  FROM atm_transactions AS a
    LEFT JOIN bank_accounts AS b
    USING (account_number)
    LEFT JOIN people AS p
       ON b.person_id = p.id
 WHERE a.year = 2021
   AND a.month = 7
   AND a.day = 28
   AND a.atm_location = "Leggett Street"
   ORDER BY p.name)
SELECT t1.name, b.activity
  FROM t1
    LEFT JOIN bakery_security_logs AS b
    USING (license_plate)
 WHERE b.year = 2021
   AND b.month = 7
   AND b.day = 28
   AND b.hour = 10
   AND b.minute <= 25
   AND b.minute >= 15
ORDER BY t1.name;

/*
| name  | activity |
| Bruce | exit     |
| Diana | exit     |
| Iman  | exit     |
| Luca  | exit
*/

-- Cross validation with phone calls
SELECT p.name AS caller, p1.name AS receiver, ph.duration
  FROM phone_calls AS ph
    JOIN people AS p
      ON ph.caller = p.phone_number
    JOIN people AS p1
      ON ph.receiver = p1.phone_number
 WHERE ph.year = 2021
   AND ph.month = 7
   AND ph.day = 28
   AND ph.duration < 60;

/*
 caller  |  receiver  | duration |
| Sofia   | Jack       | 51       |
| Kelsey  | Larry      | 36       |
| Bruce   | Robin      | 45       |
| Kelsey  | Melissa    | 50       |
| Taylor  | James      | 43       |
| Diana   | Philip     | 49       |
| Carina  | Jacqueline | 38       |
| Kenny   | Doris      | 55       |
| Benista | Anna       | 54

Now Bruce and Diana are our top suspects, and the accomplice will be either Robin or Philip
*/

-- Investigate the passengers in earliest flight out of Fiftyville in 2021.07.29

SELECT p.name, f.id, a.city AS origin, a2.city AS destination
    FROM flights AS f
    LEFT JOIN airports AS a
    ON f.origin_airport_id = a.id
    LEFT JOIN passengers AS pa
    ON f.id = pa.flight_id
    LEFT JOIN people AS p
    USING (passport_number)
    LEFT JOIN airports AS a2
    ON f.destination_airport_id = a2.id
WHERE f.id =
    (SELECT f.id
       FROM flights AS f
            LEFT JOIN airports AS a
            ON f.origin_airport_id = a.id
      WHERE a.city = "Fiftyville"
        AND f.year = 2021
        AND f.month = 7
        AND f.day = 29
   ORDER BY f.hour ASC, f.minute ASC
      LIMIT 1);

/*
|  name  | id |   origin   |  destination  |
| Doris  | 36 | Fiftyville | New York City |
| Sofia  | 36 | Fiftyville | New York City |
| Bruce  | 36 | Fiftyville | New York City |
| Edward | 36 | Fiftyville | New York City |
| Kelsey | 36 | Fiftyville | New York City |
| Taylor | 36 | Fiftyville | New York City |
| Kenny  | 36 | Fiftyville | New York City |
| Luca   | 36 | Fiftyville | New York City
*/

-- Combine all the information to close the case
WITH t2 AS (SELECT t1.person_id AS id, t1.name, b.activity
  FROM (SELECT p.name, p.license_plate, b.person_id
  FROM atm_transactions AS a
    LEFT JOIN bank_accounts AS b
    USING (account_number)
    LEFT JOIN people AS p
       ON b.person_id = p.id
 WHERE a.year = 2021
   AND a.month = 7
   AND a.day = 28
   AND a.atm_location = "Leggett Street"
   ORDER BY p.name) t1
    LEFT JOIN bakery_security_logs AS b
    USING (license_plate)
 WHERE b.year = 2021
   AND b.month = 7
   AND b.day = 28
   AND b.hour = 10
   AND b.minute <= 25
   AND b.minute >= 15
ORDER BY t1.name),
t3 as (
 SELECT p.name AS caller, p.id AS caller_id, p1.name AS receiver, p1.id AS receiver_id, ph.duration
   FROM phone_calls AS ph
    JOIN people AS p
      ON ph.caller = p.phone_number
    JOIN people AS p1
      ON ph.receiver = p1.phone_number
 WHERE ph.year = 2021
   AND ph.month = 7
   AND ph.day = 28
   AND ph.duration < 60),
t4 as (
   SELECT p.name, p.id AS person_id, f.id, a.city AS origin, a2.city AS destination
    FROM flights AS f
    LEFT JOIN airports AS a
    ON f.origin_airport_id = a.id
    LEFT JOIN passengers AS pa
    ON f.id = pa.flight_id
    LEFT JOIN people AS p
    USING (passport_number)
    LEFT JOIN airports AS a2
    ON f.destination_airport_id = a2.id
WHERE f.id =
    (SELECT f.id
       FROM flights AS f
            LEFT JOIN airports AS a
            ON f.origin_airport_id = a.id
      WHERE a.city = "Fiftyville"
        AND f.year = 2021
        AND f.month = 7
        AND f.day = 29
   ORDER BY f.hour ASC, f.minute ASC
      LIMIT 1))
SELECT t4.name AS Theft, t3.receiver AS Accomplice, t4.destination AS city_escaped_to
FROM t2
    JOIN t3
    ON t2.id = t3.caller_id
    JOIN t4
    ON t3.caller_id = t4.person_id;

/* Result
+-------+------------+-----------------+
| Theft | Accomplice | city_escaped_to |
+-------+------------+-----------------+
| Bruce | Robin      | New York City   |
+-------+------------+-----------------+
*/