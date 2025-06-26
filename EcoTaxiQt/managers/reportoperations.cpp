#include "reportoperations.h"
#include "operations.h"
#include <QDate>
#include <QVariantList>
#include <QString>

ReportOperations::ReportOperations() {}

QVariantList ReportOperations::getCarsReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "WITH grouped_events AS (\n"
        "    SELECT carId, SUM(CASE WHEN amount > 0 THEN COALESCE(amount, 0) ELSE 0 END) AS income,\n"
        "           SUM(CASE WHEN amount > 0 THEN COALESCE(amount, 0) ELSE 0 END) * 0.05 AS tax,\n"
        "           SUM(CASE WHEN amount < 0 THEN COALESCE(amount, 0) ELSE 0 END) AS outcome,\n"
        "           SUM(COALESCE(amount, 0)) AS totalAmount,\n"
        "           COUNT(DISTINCT DATE(date)) AS daysWorked,\n"
        "           COUNT(DISTINCT CASE WHEN amount != 0 THEN DATE(date) ELSE NULL END) AS nonZeroDays\n"
        "    FROM events\n"
        "    WHERE date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") +
        "'\n"
        "    GROUP BY carId\n"
        "),\n"
        "grouped_charges AS (\n"
        "    SELECT carId, SUM(COALESCE(kwh, 0)) AS totalKwhCost\n"
        "    FROM charges\n"
        "    WHERE date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") +
        "'\n"
        "    GROUP BY carId\n"
        ")\n"
        "SELECT\n"
        "    cars.id AS carId,\n"
        "    cars.sid AS carSid,\n"
        "    investors.name AS investorName,\n"
        "    COALESCE(events.income, 0) AS income,\n"
        "    FLOOR(COALESCE(events.tax, 0)) AS tax,\n"
        "    COALESCE(charges.totalKwhCost, 0) AS kwh,\n"
        "    COALESCE(events.outcome, 0) AS outcome,\n"
        "    FLOOR((events.income * 0.95) - COALESCE(charges.totalKwhCost, 0) + events.outcome) AS profit,\n"
        "    COALESCE(events.daysWorked, 0) AS daysWorked,\n"
        "    COALESCE(events.nonZeroDays, 0) AS nonZeroDays,\n"
        "    CASE WHEN events.nonZeroDays > 0 THEN \n"
        "        FLOOR(((events.income * 0.95) - COALESCE(charges.totalKwhCost, 0) + events.outcome) / events.nonZeroDays)\n"
        "    ELSE 0 END AS averageProfitPerDay,\n"
        "    COALESCE(cars.percentage, 0) AS percentage,\n"
        "    CASE WHEN ((events.income * 0.95) - COALESCE(charges.totalKwhCost, 0) + events.outcome) >= 0 THEN\n"
        "        FLOOR(COALESCE(cars.percentage, 0) * ((events.income * 0.95) - COALESCE(charges.totalKwhCost, 0) + events.outcome) / 100)\n"
        "    ELSE 0 END AS ourIncome,\n"
        "    CASE WHEN ((events.income * 0.95) - COALESCE(charges.totalKwhCost, 0) + events.outcome) >= 0 THEN\n"
        "        FLOOR((100 - COALESCE(cars.percentage, 0)) * ((events.income * 0.95) - COALESCE(charges.totalKwhCost, 0) + events.outcome) / 100)\n"
        "    ELSE FLOOR((events.income * 0.95) - COALESCE(charges.totalKwhCost, 0) + events.outcome)\n"
        "    END AS investorsIncome\n"
        "FROM cars\n"
        "LEFT JOIN grouped_events AS events ON events.carId = cars.id\n"
        "LEFT JOIN grouped_charges AS charges ON charges.carId = cars.id\n"
        "JOIN investors ON investors.id = cars.investorId;";

    result = db.executeGet(query);
    return result;
}

QVariantList ReportOperations::getAllCarsReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "WITH grouped_events AS (\n"
        "    SELECT carId, \n"
        "           SUM(CASE WHEN amount > 0 THEN COALESCE(amount, 0) ELSE 0 END) AS income,\n"
        "           SUM(CASE WHEN amount > 0 THEN COALESCE(amount, 0) ELSE 0 END) * 0.05 AS incomeTax,\n"
        "           SUM(CASE WHEN amount < 0 THEN COALESCE(amount, 0) ELSE 0 END) AS outcome,\n"
        "           SUM(COALESCE(amount, 0)) AS totalAmount\n"
        "    FROM events\n"
        "    WHERE date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") +
        "'\n"
        "    GROUP BY carId\n"
        "),\n"
        "grouped_charges AS (\n"
        "    SELECT carId, \n"
        "           SUM(COALESCE(kwh * kwh_multiplier, 0)) AS totalKwhCost\n"
        "    FROM charges\n"
        "    WHERE date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") +
        "'\n"
        "    GROUP BY carId\n"
        "),\n"
        "car_stats AS (\n"
        "    SELECT\n"
        "        cars.id AS carId,\n"
        "        COALESCE(events.income, 0) AS income,\n"
        "        COALESCE(events.incomeTax, 0) AS incomeTax,\n"
        "        COALESCE(events.outcome, 0) AS outcome,\n"
        "        FLOOR((events.income * 0.95) - COALESCE(charges.totalKwhCost, 0) + events.outcome) AS profit,\n"
        "        COALESCE(charges.totalKwhCost, 0) AS kwh,\n"
        "        COALESCE(cars.percentage, 0) AS percentage,\n"
        "        CASE \n"
        "            WHEN (events.income * 0.95) - COALESCE(charges.totalKwhCost, 0) + events.outcome >=0 THEN \n"
        "                FLOOR(cars.percentage * ((events.income * 0.95) - COALESCE(charges.totalKwhCost, 0) + events.outcome) / 100)\n"
        "            ELSE \n"
        "                0 \n"
        "        END AS ourIncome,\n"
        "        CASE \n"
        "            WHEN (events.income * 0.95) - COALESCE(charges.totalKwhCost, 0) + events.outcome > 0 THEN \n"
        "                FLOOR((100 - cars.percentage) * ((events.income * 0.95) - COALESCE(charges.totalKwhCost, 0) + events.outcome) / 100)\n"
        "            ELSE \n"
        "                FLOOR((events.income * 0.95) - COALESCE(charges.totalKwhCost, 0) + events.outcome)\n"
        "        END AS investorsIncome\n"
        "    FROM cars\n"
        "    LEFT JOIN grouped_events AS events ON cars.id = events.carId\n"
        "    LEFT JOIN grouped_charges AS charges ON cars.id = charges.carId\n"
        "    JOIN investors ON investors.id = cars.investorId\n"
        ")\n"
        "SELECT\n"
        "    FLOOR(SUM(income)) AS totalIncome,\n"
        "    FLOOR(SUM(incomeTax)) AS totalIncomeTax,\n"
        "    FLOOR(SUM(kwh)) AS totalKwhCost,\n"
        "    FLOOR(SUM(outcome)) AS totalOutcome,\n"
        "    FLOOR(SUM(profit)) AS totalProfit,\n"
        "    FLOOR(SUM(ourIncome)) AS totalOurIncome,\n"
        "    FLOOR(SUM(investorsIncome)) AS totalInvestorsIncome\n"
        "FROM car_stats";
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result.append(data[0]);
    }
    return result;
}

QVariantList ReportOperations::getTypesReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    types.id as typeId,\n"
        "    types.name as typeName,\n"
        "    SUM(CASE WHEN events.amount > 0 THEN events.amount ELSE 0 END) as income,\n"
        "    SUM(CASE WHEN events.amount < 0 THEN events.amount ELSE 0 END) as outcome,\n"
        "    SUM(events.amount) as profit\n"
        "FROM types\n"
        "JOIN events ON events.typeId = types.id\n"
        "WHERE events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "GROUP BY types.id, types.name";
    result = db.executeGet(query);
    return result;
}

QVariantList ReportOperations::getAllTypesReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "	SUM(CASE WHEN events.amount > 0 THEN events.amount ELSE 0 END) as income,\n"
        "	SUM(CASE WHEN events.amount < 0 THEN events.amount ELSE 0 END) as outcome,\n"
        "	SUM(events.amount) as profit\n"
        "FROM types\n"
        "JOIN events ON events.typeId = types.id\n"
        "WHERE events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'";
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result.append(data[0]);
    }
    return result;
}

QVariantList ReportOperations::getDriversReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "WITH EventsGrouped AS (\n"
        "    SELECT \n"
        "        driverId,\n"
        "        COUNT(id) AS events_count,\n"
        "        SUM(CASE WHEN amount > 0 THEN amount ELSE 0 END) AS income_from_events,\n"
        "        SUM(CASE WHEN amount < 0 THEN amount ELSE 0 END) AS outcome_from_events,\n"
        "        SUM(amount) AS total_event_amount\n"
        "    FROM \n"
        "        events\n"
        "    WHERE \n"
        "        date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") +
        "'\n"
        "    GROUP BY \n"
        "        driverId\n"
        "),\n"
        "ChargesGrouped AS (\n"
        "    SELECT \n"
        "        driverId,\n"
        "        SUM(kwh) AS total_kwh\n"
        "    FROM \n"
        "        charges\n"
        "    WHERE \n"
        "        date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") +
        "'\n"
        "    GROUP BY \n"
        "        driverId\n"
        ")\n"
        "SELECT \n"
        "    d.id AS driver_id,\n"
        "    d.name AS driver_name,\n"
        "    COALESCE(eg.events_count, 0) AS events_count,\n"
        "    COALESCE(eg.income_from_events, 0) AS income_from_events,\n"
        "    COALESCE(cg.total_kwh, 0) AS total_kwh,\n"
        "    COALESCE(eg.outcome_from_events, 0) AS outcome_from_events,\n"
        "    COALESCE(eg.total_event_amount, 0) AS total_profit\n"
        "FROM \n"
        "    drivers d\n"
        "LEFT JOIN \n"
        "    EventsGrouped eg ON d.id = eg.driverId\n"
        "LEFT JOIN \n"
        "    ChargesGrouped cg ON d.id = cg.driverId\n"
        "WHERE \n"
        "    eg.driverId IS NOT NULL OR cg.driverId IS NOT NULL";

    result = db.executeGet(query);
    return result;
}

QVariantList ReportOperations::getAllDriversReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString query =
        "WITH EventsGrouped AS (\n"
        "    SELECT \n"
        "        driverId,\n"
        "        COUNT(id) AS total_events_count,\n"
        "        SUM(CASE WHEN amount > 0 THEN amount ELSE 0 END) AS total_income,\n"
        "        SUM(CASE WHEN amount < 0 THEN amount ELSE 0 END) AS total_outcome,\n"
        "        SUM(amount) AS total_event_amount\n"
        "    FROM \n"
        "        events\n"
        "    WHERE \n"
        "        date BETWEEN 'start_date' AND 'end_date'\n"
        "    GROUP BY \n"
        "        driverId\n"
        "),\n"
        "ChargesGrouped AS (\n"
        "    SELECT \n"
        "        driverId,\n"
        "        SUM(kwh * kwh_multiplier) AS total_kwh\n"
        "    FROM \n"
        "        charges\n"
        "    WHERE \n"
        "        date BETWEEN 'start_date' AND 'end_date'\n"
        "    GROUP BY \n"
        "        driverId\n"
        ")\n"
        "SELECT \n"
        "    SUM(COALESCE(eg.total_events_count, 0)) AS total_events_count,\n"
        "    SUM(COALESCE(eg.total_income, 0)) AS total_income,\n"
        "    SUM(COALESCE(cg.total_kwh, 0)) AS total_kwh,\n"
        "    SUM(COALESCE(eg.total_outcome, 0)) AS total_outcome,\n"
        "    SUM(COALESCE(eg.total_event_amount, 0)) AS total_profit\n"
        "FROM \n"
        "    drivers d\n"
        "LEFT JOIN \n"
        "    EventsGrouped eg ON d.id = eg.driverId\n"
        "LEFT JOIN \n"
        "    ChargesGrouped cg ON d.id = cg.driverId\n"
        "WHERE \n"
        "    eg.driverId IS NOT NULL OR cg.driverId IS NOT NULL";

    query = query.replace("start_date", fromDate.toString("yyyy-MM-dd"));
    query = query.replace("end_date", toDate.toString("yyyy-MM-dd"));

    QVariantList data = db.executeGet(query);

    if (!data.isEmpty())
    {
        result.append(data[0]);
    }

    return result;
}

QVariantList ReportOperations::getInvestorsReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString query =
        "WITH event_stats AS (\n"
        "    SELECT\n"
        "        cars.id AS carId,\n"
        "        SUM(CASE WHEN events.amount > 0 THEN events.amount ELSE 0 END) AS income,\n"
        "        SUM(CASE WHEN events.amount > 0 THEN events.amount ELSE 0 END) * 0.05 AS incomeTax,\n"
        "        SUM(CASE WHEN events.amount < 0 THEN events.amount ELSE 0 END) AS outcome,\n"
        "        SUM(COALESCE(events.dolg, 0)) AS totalDebt\n" // добавили сумму долгов
        "    FROM events\n"
        "    LEFT JOIN cars ON cars.id = events.carId\n"
        "    WHERE events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") +
        "'\n"
        "    GROUP BY cars.id\n"
        "),\n"
        "charge_stats AS (\n"
        "    SELECT\n"
        "        cars.id AS carId,\n"
        "        SUM(COALESCE(charges.kwh * charges.kwh_multiplier, 0)) AS totalKwh\n"
        "    FROM charges\n"
        "    LEFT JOIN cars ON cars.id = charges.carId\n"
        "    WHERE charges.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") +
        "'\n"
        "    GROUP BY cars.id\n"
        ")\n"
        "SELECT\n"
        "    investors.id AS investorId,\n"
        "    investors.name AS investorName,\n"
        "    COALESCE(SUM(event_stats.income), 0) AS income,\n"
        "    COALESCE(SUM(event_stats.totalDebt), 0) AS totalDebt,\n" // добавили долг
        "    FLOOR(COALESCE(SUM(event_stats.incomeTax), 0)) AS incomeTax,\n"
        "    COALESCE(SUM(charge_stats.totalKwh), 0) AS totalKwh,\n"
        "    COALESCE(SUM(event_stats.outcome), 0) AS outcome,\n"
        "    FLOOR(SUM(\n"
        "        (COALESCE(event_stats.income, 0) * 0.95 - COALESCE(charge_stats.totalKwh, 0) + COALESCE(event_stats.outcome, 0))\n"
        "    )) AS totalProfit,\n"
        "    FLOOR(SUM(\n"
        "        CASE \n"
        "            WHEN (COALESCE(event_stats.income, 0) * 0.95 - COALESCE(charge_stats.totalKwh, 0) + COALESCE(event_stats.outcome, 0)) >= 0 THEN\n"
        "                (COALESCE(event_stats.income, 0) * 0.95 - COALESCE(charge_stats.totalKwh, 0) + COALESCE(event_stats.outcome, 0)) \n"
        "                * COALESCE(cars.percentage, 0) / 100\n"
        "            ELSE 0\n"
        "        END\n"
        "    )) AS ourIncome,\n"
        "    FLOOR(SUM(\n"
        "        CASE \n"
        "            WHEN (COALESCE(event_stats.income, 0) * 0.95 - COALESCE(charge_stats.totalKwh, 0) + COALESCE(event_stats.outcome, 0)) >= 0 THEN\n"
        "                (COALESCE(event_stats.income, 0) * 0.95 - COALESCE(charge_stats.totalKwh, 0) + COALESCE(event_stats.outcome, 0))\n"
        "                * (100 - COALESCE(cars.percentage, 0)) / 100\n"
        "            ELSE (COALESCE(event_stats.income, 0) * 0.95 - COALESCE(charge_stats.totalKwh, 0) + COALESCE(event_stats.outcome, 0))\n"
        "        END\n"
        "    )) AS investorsIncome\n"
        "FROM investors\n"
        "LEFT JOIN cars ON cars.investorId = investors.id\n"
        "LEFT JOIN event_stats ON cars.id = event_stats.carId\n"
        "LEFT JOIN charge_stats ON cars.id = charge_stats.carId\n"
        "WHERE investors.id IN (SELECT investorId FROM cars)\n"
        "GROUP BY investors.id, investors.name";

    result = db.executeGet(query);
    return result;
}

QVariantList ReportOperations::getAllInvestorsReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString query =
        "WITH grouped_events AS (\n"
        "    SELECT carId,\n"
        "           SUM(CASE WHEN amount > 0 THEN COALESCE(amount, 0) ELSE 0 END) AS income,\n"
        "           SUM(CASE WHEN amount > 0 THEN COALESCE(amount, 0) ELSE 0 END) * 0.05 AS incomeTax,\n"
        "           SUM(CASE WHEN amount < 0 THEN COALESCE(amount, 0) ELSE 0 END) AS outcome,\n"
        "           SUM(COALESCE(dolg, 0)) AS totalDebt,\n" // добавили сумму долгов
        "           SUM(COALESCE(amount, 0)) AS totalAmount\n"
        "    FROM events\n"
        "    WHERE date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") +
        "'\n"
        "    GROUP BY carId\n"
        "),\n"
        "grouped_charges AS (\n"
        "    SELECT carId,\n"
        "           SUM(COALESCE(kwh * kwh_multiplier, 0)) AS totalKwh\n"
        "    FROM charges\n"
        "    WHERE date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") +
        "'\n"
        "    GROUP BY carId\n"
        "),\n"
        "car_stats AS (\n"
        "    SELECT\n"
        "        cars.id AS carId,\n"
        "        COALESCE(events.income, 0) AS income,\n"
        "        COALESCE(events.incomeTax, 0) AS incomeTax,\n"
        "        COALESCE(events.outcome, 0) AS outcome,\n"
        "        FLOOR((events.income * 0.95) - COALESCE(charges.totalKwh, 0) + events.outcome) AS totalProfit,\n"
        "        COALESCE(charges.totalKwh, 0) AS totalKwh,\n"
        "        COALESCE(cars.percentage, 0) AS percentage,\n"
        "        COALESCE(events.totalDebt, 0) AS totalDebt,\n" // добавили долг
        "        CASE \n"
        "            WHEN (events.income * 0.95) - COALESCE(charges.totalKwh, 0) + events.outcome >= 0 THEN \n"
        "                FLOOR(cars.percentage * ((events.income * 0.95) - COALESCE(charges.totalKwh, 0) + events.outcome) / 100)\n"
        "            ELSE \n"
        "                0 \n"
        "        END AS ourIncome,\n"
        "        CASE \n"
        "            WHEN (events.income * 0.95) - COALESCE(charges.totalKwh, 0) + events.outcome > 0 THEN \n"
        "                FLOOR((100 - cars.percentage) * ((events.income * 0.95) - COALESCE(charges.totalKwh, 0) + events.outcome) / 100)\n"
        "            ELSE \n"
        "                FLOOR((events.income * 0.95) - COALESCE(charges.totalKwh, 0) + events.outcome)\n"
        "        END AS investorsIncome\n"
        "    FROM cars\n"
        "    LEFT JOIN grouped_events AS events ON cars.id = events.carId\n"
        "    LEFT JOIN grouped_charges AS charges ON cars.id = charges.carId\n"
        "    JOIN investors ON investors.id = cars.investorId\n"
        ")\n"
        "SELECT\n"
        "    FLOOR(SUM(income)) AS totalIncome,\n"
        "    FLOOR(SUM(totalDebt)) AS totalDebt,\n" // добавили итоговый долг
        "    FLOOR(SUM(incomeTax)) AS totalIncomeTax,\n"
        "    FLOOR(SUM(totalKwh)) AS totalKwhCost,\n"
        "    FLOOR(SUM(outcome)) AS totalOutcome,\n"
        "    FLOOR(SUM(totalProfit)) AS totalProfit,\n"
        "    FLOOR(SUM(ourIncome)) AS totalOurIncome,\n"
        "    FLOOR(SUM(investorsIncome)) AS totalInvestorsIncome\n"
        "FROM car_stats";

    QVariantList data = db.executeGet(query);

    if (!data.isEmpty())
    {
        result.append(data[0]);
    }

    return result;
}

QVariantList ReportOperations::getLocationsReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    locations.id as locationId,\n"
        "    locations.name as locationName,\n"
        "    SUM(charges.kwh) as kwhSum\n"
        "FROM locations\n"
        "JOIN charges ON charges.locationId = locations.id\n"
        "WHERE charges.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "GROUP BY locations.id, locations.name";
    result = db.executeGet(query);
    return result;
}

QVariantList ReportOperations::getAllLocationsReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "	SUM(charges.kwh) as kwhSum\n"
        "FROM locations\n"
        "JOIN charges ON charges.locationId = locations.id\n"
        "WHERE charges.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'";
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result.append(data[0]);
    }
    return result;
}

QVariantList ReportOperations::getChargesReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    cars.id as carId,\n"
        "    cars.sid as carSid,\n"
        "    SUM(charges.kwh) as kwhSum,\n"
        "    SUM(charges.duration) as durationSum\n"
        "FROM cars\n"
        "JOIN charges ON charges.carId = cars.id\n"
        "WHERE charges.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "GROUP BY cars.id";
    result = db.executeGet(query);
    return result;
}

QVariantList ReportOperations::getAllChargesReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "	SUM(charges.kwh) as kwhSum\n"
        "FROM cars\n"
        "JOIN charges ON charges.carId = cars.id\n"
        "WHERE charges.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'";
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result.append(data[0]);
    }
    return result;
}

QVariantList ReportOperations::getUsersReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    events.userId as id,\n"
        "    events.date as dateTime,\n"
        "    CASE WHEN events.carId = 0 THEN '-' ELSE CASE WHEN cars.id IS NULL THEN 'удален' ELSE cars.sid END END as carSid,\n"
        "    CASE WHEN events.driverId = 0 THEN '-' ELSE CASE WHEN drivers.id IS NULL THEN 'удален' ELSE drivers.name END END as driverIdAndName,\n"
        "    CASE WHEN events.typeId = 0 THEN '-' ELSE CASE WHEN types.id IS NULL THEN 'удален' ELSE types.name END END as eventTypeName,\n"
        "    events.amount as amount,\n"
        "    CASE WHEN events.userId = -1 THEN 'admin' ELSE CASE WHEN users.id IS NULL THEN 'удален' ELSE users.name END END as userName\n"
        "FROM events\n"
        "LEFT JOIN users ON users.id = events.userId\n"
        "LEFT JOIN cars ON cars.id = events.carId\n"
        "LEFT JOIN drivers ON drivers.id = events.driverId\n"
        "LEFT JOIN types ON types.id = events.typeId\n"
        "WHERE events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "ORDER BY events.date DESC";
    QVariantList data = db.executeGet(query);
    foreach (QVariant row, data)
    {
        QVariantList rowList = row.toList();
        result.append(QVariant(rowList));
    }
    return result;
}

QVariantList ReportOperations::getAllUsersReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    COUNT(*) as loginCount,\n"
        "    (SELECT COUNT(*) FROM events WHERE events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "') as eventsCount\n"
                                                                                      "FROM logins\n"
                                                                                      "WHERE logins.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") +
        "'";
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result.append(data[0]);
    }
    return result;
}

QVariantList ReportOperations::getUsers2Report(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString userQuery = "SELECT users.name FROM users;";
    QVariantList userNamesList = db.executeGet(userQuery);

    QString dynamicColumns;
    QVariantList headerRow;
    headerRow << "Дата" << "admin" << "удален";

    for (const QVariant &userNameVar : userNamesList)
    {
        QString userName = userNameVar.toList()[0].toString();
        headerRow << userName;
        dynamicColumns += QString("SUM(CASE WHEN users.name = '%1' THEN 1 ELSE 0 END) AS `%1`, ")
                              .arg(userName.replace("'", "''"));
    }
    dynamicColumns.chop(2); // Убираем последнюю запятую и пробел

    QString mainQuery = QString(
                            "SELECT DATE(events.date) AS eventDate, "
                            "SUM(CASE WHEN events.userId = -1 THEN 1 ELSE 0 END) AS admin, "
                            "SUM(CASE WHEN users.id IS NULL AND events.userId != -1 THEN 1 ELSE 0 END) AS removed "
                            "%1 "
                            "FROM events "
                            "LEFT JOIN users ON users.id = events.userId "
                            "WHERE events.date BETWEEN '" +
                            fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'"
                            "GROUP BY eventDate ORDER BY eventDate")
                            .arg(dynamicColumns);

    QVariantList data = db.executeGet(mainQuery);

    result.append(QVariant(headerRow));

    for (const QVariant &row : data)
    {
        result.append(row);
    }

    return result;
}

QVariantList ReportOperations::getAllUsers2Report(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString userQuery = "SELECT users.name FROM users;";
    QVariantList userNamesList = db.executeGet(userQuery);
    QString dynamicColumns;
    QVariantList headerRow;
    headerRow << "admin" << "удален";

    for (const QVariant &userNameVar : userNamesList)
    {
        QString userName = userNameVar.toList()[0].toString();
        headerRow << userName;
        dynamicColumns += QString("SUM(CASE WHEN users.name = '%1' THEN 1 ELSE 0 END) AS `%1`, ")
                              .arg(userName.replace("'", "''"));
    }
    dynamicColumns.chop(2); // Убираем последнюю запятую и пробел

    QString mainQuery = QString(
                            "SELECT "
                            "SUM(CASE WHEN events.userId = -1 THEN 1 ELSE 0 END) AS admin, "
                            "SUM(CASE WHEN users.id IS NULL AND events.userId != -1 THEN 1 ELSE 0 END) AS удален "
                            "%1 "
                            "FROM events "
                            "LEFT JOIN users ON users.id = events.userId "
                            "WHERE events.date BETWEEN '" +
                            fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'")
                            .arg(dynamicColumns);

    QVariantList data = db.executeGet(mainQuery);

    result.append(QVariant(headerRow));

    if (!data.isEmpty())
    {
        QVariantList rowList = data[0].toList();
        result.append(QVariant(rowList));
    }
    return result;
}

QVariantList ReportOperations::getDebtsReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString query =
        "SELECT\n"
        "   events.id,\n"
        "   cars.licensePlate,\n"
        "   investors.name AS investorName,\n"
        "   drivers.name AS driverName,\n"
        "   events.dolg AS debtAmount,\n"
        "   DATE_FORMAT(events.date, '%Y-%m-%d') AS debtDate\n" // добавляем дату в формате YYYY-MM-DD
        "FROM events\n"
        "LEFT JOIN cars ON events.carId = cars.id\n"
        "LEFT JOIN types ON events.typeId = types.id\n"
        "LEFT JOIN investors ON investors.id = cars.investorId\n"
        "LEFT JOIN drivers ON events.driverId = drivers.id\n"
        "WHERE types.name = 'Аренда'\n"
        "AND events.dolg > 0\n"
        "AND events.description NOT LIKE 'долг оплачен%'\n"
        "AND events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
        "ORDER BY events.date DESC";

    result = db.executeGet(query);
    return result;
}

QVariantList ReportOperations::getAllDebtsReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString query =
        "SELECT\n"
        "   COUNT(*) AS rentCount,\n"
        "   SUM(COALESCE(events.dolg, 0)) AS totalDebtAmount\n"
        "FROM events\n"
        "JOIN types ON events.typeId = types.id\n"
        "WHERE events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
        "AND types.name = 'Аренда'";

    result = db.executeGet(query);
    return result;
}

QVariantList ReportOperations::getFinesByCarsReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    dbManager &db = dbManager::getInstance();

    QString query =
        "SELECT\n"
        "    cars.id AS carId,\n"
        "    cars.licensePlate AS carLicensePlate,\n"
        "    COALESCE(SUM(fines.amount), 0) AS totalFinesAmount,\n"
        "    COALESCE(COUNT(fines.id), 0) AS totalFinesCount,\n"
        "    COALESCE(SUM(CASE WHEN fines.isPaid THEN fines.amount ELSE 0 END), 0) AS paidFinesAmount,\n"
        "    COALESCE(SUM(CASE WHEN fines.isPaid THEN 1 ELSE 0 END), 0) AS paidFinesCount,\n"
        "    COALESCE(SUM(CASE WHEN NOT fines.isPaid THEN fines.amount ELSE 0 END), 0) AS notPaidFinesAmount,\n"
        "    COALESCE(SUM(CASE WHEN NOT fines.isPaid THEN 1 ELSE 0 END), 0) AS notPaidFinesCount\n"
        "FROM cars\n"
        "LEFT JOIN fines ON fines.carId = cars.id\n"
        "WHERE fines.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "GROUP BY cars.id, cars.licensePlate\n";

    QVariantList result = db.executeGet(query);

    return result;
}

QVariantList ReportOperations::getAllFinesByCarsReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    dbManager &db = dbManager::getInstance();

    QString query =
        "SELECT\n"
        "    COALESCE(SUM(fines.amount), 0) AS totalFinesAmount,\n"
        "    COALESCE(COUNT(fines.id), 0) AS totalFinesCount,\n"
        "    COALESCE(SUM(CASE WHEN fines.isPaid THEN fines.amount ELSE 0 END), 0) AS paidFinesAmount,\n"
        "    COALESCE(SUM(CASE WHEN fines.isPaid THEN 1 ELSE 0 END), 0) AS paidFinesCount,\n"
        "    COALESCE(SUM(CASE WHEN NOT fines.isPaid THEN fines.amount ELSE 0 END), 0) AS notPaidFinesAmount,\n"
        "    COALESCE(SUM(CASE WHEN NOT fines.isPaid THEN 1 ELSE 0 END), 0) AS notPaidFinesCount\n"
        "FROM cars\n"
        "LEFT JOIN fines ON fines.carId = cars.id\n"
        "WHERE fines.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n";

    QVariantList result = db.executeGet(query);

    return result;
}

QVariantList ReportOperations::getFinesByDriversReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    dbManager &db = dbManager::getInstance();

    QString query =
        "SELECT\n"
        "    drivers.id AS driverId,\n"
        "    drivers.name AS driverName,\n"
        "    COALESCE(SUM(fines.amount), 0) AS totalFinesAmount,\n"
        "    COALESCE(COUNT(fines.id), 0) AS totalFinesCount,\n"
        "    COALESCE(SUM(CASE WHEN fines.isPaid THEN fines.amount ELSE 0 END), 0) AS paidFinesAmount,\n"
        "    COALESCE(SUM(CASE WHEN fines.isPaid THEN 1 ELSE 0 END), 0) AS paidFinesCount,\n"
        "    COALESCE(SUM(CASE WHEN NOT fines.isPaid THEN fines.amount ELSE 0 END), 0) AS notPaidFinesAmount,\n"
        "    COALESCE(SUM(CASE WHEN NOT fines.isPaid THEN 1 ELSE 0 END), 0) AS notPaidFinesCount\n"
        "FROM drivers\n"
        "LEFT JOIN fines ON fines.driverId = drivers.id\n"
        "WHERE fines.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "GROUP BY drivers.id, drivers.name\n";

    QVariantList result = db.executeGet(query);

    return result;
}

QVariantList ReportOperations::getAllFinesByDriversReport(QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    dbManager &db = dbManager::getInstance();

    QString query =
        "SELECT\n"
        "    COALESCE(SUM(fines.amount), 0) AS totalFinesAmount,\n"
        "    COALESCE(COUNT(fines.id), 0) AS totalFinesCount,\n"
        "    COALESCE(SUM(CASE WHEN fines.isPaid THEN fines.amount ELSE 0 END), 0) AS paidFinesAmount,\n"
        "    COALESCE(SUM(CASE WHEN fines.isPaid THEN 1 ELSE 0 END), 0) AS paidFinesCount,\n"
        "    COALESCE(SUM(CASE WHEN NOT fines.isPaid THEN fines.amount ELSE 0 END), 0) AS notPaidFinesAmount,\n"
        "    COALESCE(SUM(CASE WHEN NOT fines.isPaid THEN 1 ELSE 0 END), 0) AS notPaidFinesCount\n"
        "FROM drivers\n"
        "LEFT JOIN fines ON fines.driverId = drivers.id\n"
        "WHERE fines.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n";

    QVariantList result = db.executeGet(query);

    return result;
}

///////////////////////////// reports for item /////////////////////////////////////////////////////
QVariantList ReportOperations::getCarReport(int carId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    events.date as eventDate,\n"
        "    CASE WHEN events.typeId = 0 THEN '-' ELSE CASE WHEN types.id IS NULL THEN 'удален' ELSE types.name END END as eventTypeName,\n"
        "    CASE WHEN events.driverId = 0 THEN '-' ELSE CASE WHEN drivers.id IS NULL THEN 'удален' ELSE drivers.name END END as driverIdAndName,\n"
        "    events.amount as eventAmount,\n"
        "    events.dolg as eventDebt,\n"
        "    events.description as eventDescription\n"
        "FROM events\n"
        "LEFT JOIN drivers ON drivers.id = events.driverId\n"
        "LEFT JOIN types ON types.id = events.typeId\n"
        "WHERE events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
        "AND events.carId = " +
        QString::number(carId) +
        "\nORDER BY events.date DESC";
    result = db.executeGet(query);
    return result;
}

QVariantList ReportOperations::getAllCarReport(int carId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "WITH grouped_events AS (\n"
        "    SELECT \n"
        "        carId,\n"
        "        COALESCE(SUM(CASE WHEN amount > 0 THEN amount ELSE 0 END), 0) AS totalIncome,\n"
        "        COALESCE(SUM(CASE WHEN amount > 0 THEN amount ELSE 0 END) * 0.05, 0) AS totalIncomeTax,\n"
        "        COALESCE(SUM(CASE WHEN amount < 0 THEN amount ELSE 0 END), 0) AS totalOutcome,\n"
        "        COALESCE(SUM(CASE WHEN amount > 0 THEN amount * 0.95 ELSE 0 END), 0) AS totalEventProfit,\n"
        "        COALESCE(SUM(dolg), 0) AS totalDebt\n" // <--- добавили сумму долгов
        "    FROM events\n"
        "    WHERE carId = " +
        QString::number(carId) + "\n"
                                 "        AND date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") +
        "'\n"
        "    GROUP BY carId\n"
        "),\n"
        "grouped_charges AS (\n"
        "    SELECT \n"
        "        carId,\n"
        "        SUM(COALESCE(kwh * kwh_multiplier, 0)) AS totalKwh\n"
        "    FROM charges\n"
        "    WHERE carId = " +
        QString::number(carId) + "\n"
                                 "        AND date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") +
        "'\n"
        "    GROUP BY carId\n"
        ")\n"
        "SELECT\n"
        "    COALESCE(e.totalIncome, 0) AS totalIncome,\n"
        "    FLOOR(COALESCE(e.totalIncomeTax, 0)) AS totalIncomeTax,\n"
        "    COALESCE(c.totalKwh, 0) AS totalKwh,\n"
        "    COALESCE(e.totalOutcome, 0) AS totalOutcome,\n"
        "    FLOOR(COALESCE(e.totalEventProfit, 0) - COALESCE(c.totalKwh, 0) + COALESCE(e.totalOutcome, 0)) AS totalAmount,\n"
        "    COALESCE(cars.percentage, 0) AS percentage,\n"
        "    CASE \n"
        "        WHEN (COALESCE(e.totalEventProfit, 0) - COALESCE(c.totalKwh, 0) + COALESCE(e.totalOutcome, 0)) < 0 THEN \n"
        "            0 \n"
        "        ELSE \n"
        "            FLOOR((COALESCE(e.totalEventProfit, 0) - COALESCE(c.totalKwh, 0) + COALESCE(e.totalOutcome, 0)) * COALESCE(cars.percentage, 0) / 100) \n"
        "    END AS ourProfit,\n"
        "    CASE \n"
        "        WHEN (COALESCE(e.totalEventProfit, 0) - COALESCE(c.totalKwh, 0) + COALESCE(e.totalOutcome, 0)) < 0 THEN \n"
        "            FLOOR(COALESCE(e.totalEventProfit, 0) - COALESCE(c.totalKwh, 0) + COALESCE(e.totalOutcome, 0)) \n"
        "        ELSE \n"
        "            FLOOR((COALESCE(e.totalEventProfit, 0) - COALESCE(c.totalKwh, 0) + COALESCE(e.totalOutcome, 0)) * (100 - COALESCE(cars.percentage, 0)) / 100) \n"
        "    END AS investorsProfit,\n"
        "    COALESCE(e.totalDebt, 0) AS totalDebt\n" // <--- добавили вывод суммы долгов
        "FROM cars\n"
        "LEFT JOIN grouped_events AS e ON cars.id = e.carId\n"
        "LEFT JOIN grouped_charges AS c ON cars.id = c.carId\n"
        "WHERE cars.id = " +
        QString::number(carId) + ";";

    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result = data[0].toList();
    }
    return result;
}

QVariantList ReportOperations::getTypeReport(int typeId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    events.date as eventDate,\n"
        "    CASE WHEN events.driverId = 0 THEN '-' ELSE CASE WHEN drivers.id IS NULL THEN 'удален' ELSE drivers.name END END as driverName,\n"
        "    CASE WHEN events.carId = 0 THEN '-' ELSE CASE WHEN cars.id IS NULL THEN 'удалена' ELSE cars.sid END END as carId,\n"
        "    events.amount as eventAmount,\n"
        "    events.description as eventDescription\n"
        "FROM events\n"
        "LEFT JOIN drivers ON drivers.id = events.driverId\n"
        "LEFT JOIN cars ON cars.id = events.carId\n"
        "WHERE events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "AND events.typeId = " +
        QString::number(typeId) +
        "\nORDER BY events.date DESC";
    result = db.executeGet(query);
    return result;
}

QVariantList ReportOperations::getAllTypeReport(int typeId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    SUM(CASE WHEN events.amount > 0 THEN events.amount ELSE 0 END) as totalIncome,\n"
        "    SUM(CASE WHEN events.amount < 0 THEN events.amount ELSE 0 END) as totalOutcome,\n"
        "    SUM(events.amount) as totalAmount\n"
        "FROM events\n"
        "WHERE events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "AND events.typeId = " +
        QString::number(typeId);
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result.append(data[0].toList());
    }
    qDebug() << result;
    return result;
}

QVariantList ReportOperations::getDriverReport(int driverId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    events.date as eventDate,\n"
        "    CASE WHEN events.typeId = 0 THEN '-' ELSE CASE WHEN types.id IS NULL THEN 'удален' ELSE types.name END END as eventTypeName,\n"
        "    CASE WHEN events.carId = 0 THEN '-' ELSE CASE WHEN cars.id IS NULL THEN 'удалена' ELSE cars.sid END END as carId,\n"
        "    events.amount as eventAmount,\n"
        "    events.description as eventDescription\n"
        "FROM events\n"
        "JOIN drivers ON drivers.id = events.driverId\n"
        "LEFT JOIN cars ON cars.id = events.carId\n"
        "LEFT JOIN types ON types.id = events.typeId\n"
        "WHERE events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "AND events.driverId = " +
        QString::number(driverId) +
        "\nORDER BY events.date DESC";
    result = db.executeGet(query);
    return result;
}

QVariantList ReportOperations::getAllDriverReport(int driverId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString query =
        "WITH ChargesGrouped AS (\n"
        "    SELECT driverId, SUM(COALESCE(kwh * kwh_multiplier, 0)) AS totalKwhCost\n"
        "    FROM charges\n"
        "    WHERE driverId = " +
        QString::number(driverId) + "\n"
                                    "    AND date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") +
        "'\n"
        "    GROUP BY driverId\n"
        "),\n"
        "EventsGrouped AS (\n"
        "    SELECT driverId, SUM(CASE WHEN amount > 0 THEN amount ELSE 0 END) AS totalIncome,\n"
        "           SUM(CASE WHEN amount < 0 THEN amount ELSE 0 END) AS totalOutcome,\n"
        "           SUM(amount) AS totalAmount\n"
        "    FROM events\n"
        "    WHERE driverId = " +
        QString::number(driverId) + "\n"
                                    "    AND date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") +
        "'\n"
        "    GROUP BY driverId\n"
        ")\n"
        "SELECT\n"
        "    COALESCE(eg.totalIncome, 0) AS totalIncome,\n"
        "    COALESCE(cg.totalKwhCost, 0) AS totalKwh,\n"
        "    COALESCE(eg.totalOutcome, 0) AS totalOutcome,\n"
        "    COALESCE(eg.totalAmount, 0) AS totalAmount\n"
        "FROM drivers d\n"
        "LEFT JOIN EventsGrouped eg ON d.id = eg.driverId\n"
        "LEFT JOIN ChargesGrouped cg ON d.id = cg.driverId\n"
        "WHERE d.id = " +
        QString::number(driverId) + "\n"
                                    "GROUP BY d.id;";

    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result = data[0].toList();
    }
    return result;
}

QVariantList ReportOperations::getInvestorReport(int investorId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "WITH event_stats AS (\n"
        "    SELECT\n"
        "        carId,\n"
        "        SUM(CASE WHEN amount > 0 THEN amount ELSE 0 END) as income,\n"
        "        SUM(CASE WHEN amount > 0 THEN amount ELSE 0 END) * 0.05 as incomeTax,\n"
        "        SUM(CASE WHEN amount < 0 THEN amount ELSE 0 END) as outcome,\n"
        "        SUM(COALESCE(dolg, 0)) as totalDebt\n"
        "    FROM events\n"
        "    WHERE date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") +
        "'\n"
        "    GROUP BY carId\n"
        "),\n"
        "charge_stats AS (\n"
        "    SELECT\n"
        "        carId,\n"
        "        SUM(COALESCE(kwh, 0)) as kwh,\n"
        "        SUM(COALESCE(kwh * kwh_multiplier, 0)) as kwh_with_multiplier\n"
        "    FROM charges\n"
        "    WHERE date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") +
        "'\n"
        "    GROUP BY carId\n"
        "),\n"
        "car_stats AS (\n"
        "    SELECT\n"
        "        cars.id as carId,\n"
        "        cars.sid as carSid,\n"
        "        COALESCE(event_stats.income, 0) as income,\n"
        "        FLOOR(COALESCE(event_stats.incomeTax, 0)) as incomeTax,\n"
        "        COALESCE(event_stats.outcome, 0) as outcome,\n"
        "        COALESCE(charge_stats.kwh, 0) as kwh,\n"
        "        COALESCE(charge_stats.kwh_with_multiplier, 0) as kwh_with_multiplier,\n"
        "        COALESCE(cars.percentage, 0) as percentage,\n"
        "        COALESCE(event_stats.totalDebt, 0) as totalDebt\n"
        "    FROM investors\n"
        "    JOIN cars ON cars.investorId = investors.id\n"
        "    LEFT JOIN event_stats ON cars.id = event_stats.carId\n"
        "    LEFT JOIN charge_stats ON cars.id = charge_stats.carId\n"
        "    WHERE investors.id = " +
        QString::number(investorId) +
        "\n"
        ")\n"
        "SELECT\n"
        "    carId,\n"
        "    carSid,\n"
        "    income,\n"
        "    incomeTax,\n"
        "    kwh,\n"
        "    outcome,\n"
        "    FLOOR(income * 0.95 - kwh_with_multiplier + outcome) as profit,\n"
        "    percentage,\n"
        "    CASE \n"
        "        WHEN (FLOOR(income * 0.95 - kwh_with_multiplier + outcome) < 0) THEN \n"
        "            0 \n"
        "        ELSE \n"
        "            FLOOR((income * 0.95 - kwh_with_multiplier + outcome) * COALESCE(percentage, 0) / 100) \n"
        "    END AS ourProfit,\n"
        "    CASE \n"
        "        WHEN (FLOOR(income * 0.95 - kwh_with_multiplier + outcome) < 0) THEN \n"
        "            FLOOR(income * 0.95 - kwh_with_multiplier + outcome) \n"
        "        ELSE \n"
        "            FLOOR((income * 0.95 - kwh_with_multiplier + outcome) * COALESCE(100 - percentage, 0) / 100) \n"
        "    END AS investorsProfit,\n"
        "    totalDebt\n"
        "FROM car_stats;";

    result = db.executeGet(query);
    return result;
}

QVariantList ReportOperations::getAllInvestorReport(int investorId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "WITH event_stats AS (\n"
        "    SELECT\n"
        "        carId,\n"
        "        SUM(CASE WHEN amount > 0 THEN amount ELSE 0 END) AS income,\n"
        "        SUM(CASE WHEN amount > 0 THEN amount ELSE 0 END) * 0.05 AS incomeTax,\n"
        "        SUM(CASE WHEN amount < 0 THEN amount ELSE 0 END) AS outcome,\n"
        "        SUM(COALESCE(dolg, 0)) AS totalDebt\n"
        "    FROM events\n"
        "    WHERE date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") +
        "'\n"
        "    GROUP BY carId\n"
        "),\n"
        "charge_stats AS (\n"
        "    SELECT\n"
        "        carId,\n"
        "        SUM(COALESCE(kwh * kwh_multiplier, 0)) AS kwh\n"
        "    FROM charges\n"
        "    WHERE date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") +
        "'\n"
        "    GROUP BY carId\n"
        "),\n"
        "car_stats AS (\n"
        "    SELECT\n"
        "        cars.id AS carId,\n"
        "        investors.name AS investorName,\n"
        "        COALESCE(event_stats.income, 0) AS income,\n"
        "        COALESCE(event_stats.incomeTax, 0) AS incomeTax,\n"
        "        COALESCE(event_stats.outcome, 0) AS outcome,\n"
        "        COALESCE(charge_stats.kwh, 0) AS kwh,\n"
        "        COALESCE(cars.percentage, 0) AS percentage,\n"
        "        FLOOR(COALESCE(event_stats.income, 0) * 0.95 + COALESCE(event_stats.outcome, 0) - COALESCE(charge_stats.kwh, 0)) AS profit,\n"
        "        FLOOR(CASE \n"
        "            WHEN (COALESCE(event_stats.income, 0) * 0.95 + COALESCE(event_stats.outcome, 0) - COALESCE(charge_stats.kwh, 0)) < 0 THEN 0 \n"
        "            ELSE (COALESCE(event_stats.income, 0) * 0.95 + COALESCE(event_stats.outcome, 0) - COALESCE(charge_stats.kwh, 0)) * (COALESCE(cars.percentage, 0) / 100) \n"
        "        END) AS ourProfit,\n"
        "        FLOOR(CASE \n"
        "            WHEN (COALESCE(event_stats.income, 0) * 0.95 + COALESCE(event_stats.outcome, 0) - COALESCE(charge_stats.kwh, 0)) < 0 THEN \n"
        "                (COALESCE(event_stats.income, 0) * 0.95 + COALESCE(event_stats.outcome, 0) - COALESCE(charge_stats.kwh, 0)) \n"
        "            ELSE \n"
        "                (COALESCE(event_stats.income, 0) * 0.95 + COALESCE(event_stats.outcome, 0) - COALESCE(charge_stats.kwh, 0)) * ((100 - COALESCE(cars.percentage, 0)) / 100) \n"
        "        END) AS investorsProfit,\n"
        "        COALESCE(event_stats.totalDebt, 0) AS totalDebt\n"
        "    FROM investors\n"
        "    LEFT JOIN cars ON cars.investorId = investors.id\n"
        "    LEFT JOIN event_stats ON cars.id = event_stats.carId\n"
        "    LEFT JOIN charge_stats ON cars.id = charge_stats.carId\n"
        "    WHERE investors.id = " +
        QString::number(investorId) +
        "\n"
        ")\n"
        "SELECT\n"
        "    investorName,\n"
        "    FLOOR(SUM(income)) AS totalIncome,\n"
        "    FLOOR(SUM(incomeTax)) AS totalIncomeTax,\n"
        "    FLOOR(SUM(kwh)) AS totalKwh,\n"
        "    FLOOR(SUM(outcome)) AS totalOutcome,\n"
        "    FLOOR(SUM(profit)) AS totalProfit,\n"
        "    COUNT(carId) AS totalCarsCount,\n"
        "    FLOOR(SUM(ourProfit)) AS totalOurIncome,\n"
        "    FLOOR(SUM(investorsProfit)) AS totalInvestorsIncome,\n"
        "    FLOOR(SUM(totalDebt)) AS totalDebt\n"
        "FROM car_stats\n"
        "GROUP BY investorName;";
    QVariantList data = db.executeGet(query);
    qDebug() << data;

    if (!data.isEmpty())
    {
        QVariantList rowData = data[0].toList();
        result.append(rowData);
    }
    else
    {
        result.append({0, 0, 0, 0, 0, 0, 0, 0, 0});
    }

    return result;
}

QVariantList ReportOperations::getLocationReport(int locationId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    charges.date as date,\n"
        "    CASE WHEN cars.id IS NULL THEN 'удалена' ELSE cars.sid END as carId,\n"
        "    CASE WHEN drivers.id IS NULL THEN 'удален' ELSE drivers.name END as driverIdAndName,\n"
        "    charges.kwh as kwh,\n"
        "    charges.duration as duration\n"
        "FROM locations\n"
        "JOIN charges ON charges.locationId = locations.id\n"
        "LEFT JOIN cars ON cars.id = charges.carId\n"
        "LEFT JOIN drivers ON drivers.id = charges.driverId\n"
        "WHERE charges.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "AND locations.id = " +
        QString::number(locationId) +
        " ORDER BY charges.date DESC";
    result = db.executeGet(query);
    return result;
}

QVariantList ReportOperations::getAllLocationReport(int locationId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    SUM(charges.kwh) as kwhSum\n"
        "FROM locations\n"
        "JOIN charges ON charges.locationId = locations.id\n"
        "WHERE charges.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "AND locations.id = " +
        QString::number(locationId);
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result = data[0].toList();
    }
    return result;
}

QVariantList ReportOperations::getChargesByCarReport(int id, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    charges.date as chargeDate,\n"
        "    drivers.name as chargeDriver,\n"
        "    locations.name as locationName,\n"
        "    charges.kwh as kwh,\n"
        "    charges.duration as duration\n"
        "FROM cars\n"
        "JOIN charges ON cars.id = charges.carId\n"
        "JOIN drivers ON drivers.id = charges.driverId\n"
        "JOIN locations ON locations.id = charges.locationId\n"
        "WHERE charges.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "AND cars.id = " +
        QString::number(id) + "\n"
                              "ORDER BY charges.date DESC";
    result = db.executeGet(query);
    return result;
}

QVariantList ReportOperations::getAllChargesByCarReport(int id, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    SUM(charges.kwh) as kwhSum\n"
        "FROM cars\n"
        "JOIN charges ON cars.id = charges.carId\n"
        "WHERE charges.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n"
                                                                                      "AND cars.id = " +
        QString::number(id);
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result = data[0].toList();
    }
    return result;
}

QVariantList ReportOperations::getUserReport(int userId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    events.id as id,\n"
        "    events.date as datetime,\n"
        "    CASE WHEN events.typeId = 0 THEN '-' ELSE CASE WHEN types.id IS NULL THEN 'удален' ELSE types.name END END as type,\n"
        "    CASE WHEN events.driverId = 0 THEN '-' ELSE CASE WHEN drivers.id IS NULL THEN 'удален' ELSE drivers.name END END as driverName,\n"
        "    CASE WHEN events.carId = 0 THEN '-' ELSE CASE WHEN cars.id IS NULL THEN 'удалена' ELSE cars.sid END END as carSid,\n"
        "    events.amount as amount\n"
        "FROM events\n"
        "LEFT JOIN drivers ON drivers.id = events.driverId\n"
        "LEFT JOIN cars ON cars.id = events.carId\n"
        "LEFT JOIN types ON types.id = events.typeId\n"
        "WHERE events.userId = " +
        QString::number(userId) + "\n"
                                  "  AND events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd HH:mm:ss") + "' AND '" + toDate.toString("yyyy-MM-dd HH:mm:ss") + "'\n"
                                                                                                        "ORDER BY events.date DESC";
    result = db.executeGet(query);
    return result;
}

QVariantList ReportOperations::getAllUserReport(int userId, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    COUNT(*) as loginCount,\n"
        "    (SELECT COUNT(*) FROM events WHERE events.userId = " +
        QString::number(userId) + " AND events.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd HH:mm:ss") + "' AND '" + toDate.toString("yyyy-MM-dd HH:mm:ss") + "') as eventCount\n"
                                                                                                        "FROM logins\n"
                                                                                                        "WHERE logins.userId = " +
        QString::number(userId) + "\n"
                                  "  AND logins.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd HH:mm:ss") + "' AND '" + toDate.toString("yyyy-MM-dd HH:mm:ss") + "'";
    QVariantList data = db.executeGet(query);
    if (!data.isEmpty())
    {
        result.append(data[0]);
    }
    return result;
}

QVariantList ReportOperations::getFinesByCarReport(int id, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    fines.id,\n"
        "    fines.date,\n"
        "    drivers.name,\n"
        "    fines.time,\n"
        "    fines.FID,\n"
        "    fines.amount,\n"
        "    fines.isPaid,\n"
        "    fines.description\n"
        "FROM fines\n"
        "LEFT JOIN drivers ON fines.driverId = drivers.id\n"
        "WHERE fines.carId = " +
        QString::number(id) + "\n"
                              "AND fines.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'"
                                                                                      "ORDER BY fines.date DESC";

    QVariantList result = db.executeGet(query);

    return result;
}

QVariantList ReportOperations::getAllFinesByCarReport(int id, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    COALESCE(SUM(fines.amount), 0) AS totalFinesAmount,\n"
        "    COALESCE(COUNT(fines.id), 0) AS totalFinesCount,\n"
        "    COALESCE(SUM(CASE WHEN fines.isPaid THEN fines.amount ELSE 0 END), 0) AS paidFinesAmount,\n"
        "    COALESCE(SUM(CASE WHEN fines.isPaid THEN 1 ELSE 0 END), 0) AS paidFinesCount,\n"
        "    COALESCE(SUM(CASE WHEN NOT fines.isPaid THEN fines.amount ELSE 0 END), 0) AS notPaidFinesAmount,\n"
        "    COALESCE(SUM(CASE WHEN NOT fines.isPaid THEN 1 ELSE 0 END), 0) AS notPaidFinesCount\n"
        "FROM cars\n"
        "LEFT JOIN fines ON fines.carId = cars.id\n"
        "WHERE cars.id = " +
        QString::number(id) + "\n"
                              "AND fines.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n";

    QVariantList result = db.executeGet(query);

    return result;
}

QVariantList ReportOperations::getFinesByDriverReport(int id, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    fines.id,\n"
        "    fines.date,\n"
        "    cars.sid,\n"
        "    fines.time,\n"
        "    fines.FID,\n"
        "    fines.amount,\n"
        "    fines.isPaid,\n"
        "    fines.description\n"
        "FROM fines\n"
        "LEFT JOIN cars ON fines.carId = cars.id\n"
        "WHERE fines.driverId = " +
        QString::number(id) + "\n"
                              "AND fines.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'"
                                                                                      "ORDER BY fines.date DESC";

    QVariantList result = db.executeGet(query);

    return result;
}

QVariantList ReportOperations::getAllFinesByDriverReport(int id, QDate fromDate, QDate toDate)
{
    toDate = toDate.addDays(1);
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT\n"
        "    COALESCE(SUM(fines.amount), 0) AS totalFinesAmount,\n"
        "    COALESCE(COUNT(fines.id), 0) AS totalFinesCount,\n"
        "    COALESCE(SUM(CASE WHEN fines.isPaid THEN fines.amount ELSE 0 END), 0) AS paidFinesAmount,\n"
        "    COALESCE(SUM(CASE WHEN fines.isPaid THEN 1 ELSE 0 END), 0) AS paidFinesCount,\n"
        "    COALESCE(SUM(CASE WHEN NOT fines.isPaid THEN fines.amount ELSE 0 END), 0) AS notPaidFinesAmount,\n"
        "    COALESCE(SUM(CASE WHEN NOT fines.isPaid THEN 1 ELSE 0 END), 0) AS notPaidFinesCount\n"
        "FROM drivers\n"
        "LEFT JOIN fines ON fines.driverId = drivers.id\n"
        "WHERE drivers.id = " +
        QString::number(id) + "\n"
                              "AND fines.date BETWEEN '" +
        fromDate.toString("yyyy-MM-dd") + "' AND '" + toDate.toString("yyyy-MM-dd") + "'\n";

    QVariantList result = db.executeGet(query);

    return result;
}

///////////////////////////// other reports /////////////////////////////////////////////////////

QVariantList ReportOperations::getRepairsReport()
{
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT "
        "   repairs.id,\n"
        "   cars.id AS carId,\n"
        "   cars.licensePlate AS carLicensePlate,\n"
        "   (SELECT investors.name FROM investors WHERE investors.id = cars.investorId) AS carInvestor,\n"
        "   COALESCE(DATEDIFF(IFNULL(repairs.toDate, CURRENT_DATE()), repairs.fromDate), 0) AS daysCount,\n"
        "   IFNULL(repairs.fromDate, '-') AS fromDate,\n"
        "   IFNULL(repairs.toDate, '-') AS toDate,\n"
        "   repairs.description\n"
        "FROM repairs\n"
        "LEFT JOIN cars ON cars.id = repairs.carId\n"
        "ORDER BY repairs.id DESC";
    QVariantList data = db.executeGet(query);
    return data;
}

QVariantList ReportOperations::getNotDoneRepairsReport()
{
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT "
        "   repairs.id,\n"
        "   cars.sid AS carId,\n"
        "   (SELECT investors.name FROM investors WHERE investors.id = cars.investorId) AS carInvestor,\n"
        "   COALESCE(DATEDIFF(IFNULL(repairs.toDate, CURRENT_DATE()), repairs.fromDate), 0) AS daysCount,\n"
        "   IFNULL(repairs.fromDate, '-') AS fromDate,\n"
        "   IFNULL(repairs.toDate, '-') AS toDate,\n"
        "   repairs.description\n"
        "FROM repairs\n"
        "LEFT JOIN cars ON cars.id = repairs.carId\n"
        "WHERE repairs.toDate IS NULL\n"
        "ORDER BY repairs.id DESC";
    QVariantList data = db.executeGet(query);
    return data;
}

QVariantList ReportOperations::getFinesReport()
{
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT "
        "   fines.id,\n"
        "   fines.date AS date,\n"
        "   CASE WHEN fines.carId = 0 THEN '-' ELSE CASE WHEN cars.id IS NULL THEN 'удален' ELSE cars.licensePlate END END AS carLicensePlate,\n"
        "   CASE WHEN fines.driverId = 0 THEN '-' ELSE CASE WHEN drivers.id IS NULL THEN 'удален' ELSE drivers.name END END AS driverName,\n"
        "   fines.time AS time,\n"
        "   fines.FID AS fid,\n"
        "   fines.amount AS fineAmount,\n"
        "   fines.isPaid AS isPaid,\n"
        "   fines.description AS description\n"
        "FROM fines\n"
        "LEFT JOIN cars ON cars.id = fines.carId\n"
        "LEFT JOIN drivers ON drivers.id = fines.driverId\n"
        "ORDER BY fines.id DESC";
    QVariantList data = db.executeGet(query);
    return data;
}

QVariantList ReportOperations::getNotPaidFinesReport()
{
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT "
        "   fines.id,\n"
        "   fines.date AS date,\n"
        "   CASE WHEN fines.carId = 0 THEN '-' ELSE CASE WHEN cars.id IS NULL THEN 'удален' ELSE cars.licensePlate END END AS carLicensePlate,\n"
        "   CASE WHEN fines.driverId = 0 THEN '-' ELSE CASE WHEN drivers.id IS NULL THEN 'удален' ELSE drivers.name END END AS driverName,\n"
        "   fines.time AS time,\n"
        "   fines.FID AS fid,\n"
        "   fines.amount AS fineAmount,\n"
        "   fines.isPaid AS isPaid,\n"
        "   fines.description AS description\n"
        "FROM fines\n"
        "LEFT JOIN cars ON cars.id = fines.carId\n"
        "LEFT JOIN drivers ON drivers.id = fines.driverId\n"
        "WHERE fines.isPaid = 0\n"
        "ORDER BY fines.id DESC";
    QVariantList data = db.executeGet(query);
    return data;
}
