token = undefined

if (localStorage.getItem('token')) {
  token = localStorage.getItem('token')
  openMenu()
}

function checkPassword(event) {
  event.preventDefault()
  const password = document.getElementById('password').value
  document.getElementById('password').value = ''
  token = password
  openMenu()
}

// report menu
async function openMenu() {
  from = undefined
  to = undefined
  if (document.getElementById('events-start-date').value) {
    from = new Date(document.getElementById('events-start-date').value)
    to = new Date(document.getElementById('events-end-date').value)
  }
  else {
    from = new Date()
    from.setDate(from.getDate() - 1)
    to = new Date()
  }
  await loadReport(from, to)
  showMenu(from, to)
}

async function loadReport(from, to) {
  document.body.style.cursor='wait'
  await fetch('https://85wc02tqsc.execute-api.eu-north-1.amazonaws.com/ecotaxi-investor/getInvestorReport', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify({ password: token, from: from.toISOString().slice(0, 10), to: to.toISOString().slice(0, 10) })
  })

    .then(response => {
      return response.json()
    })
    .then(data => {
      if (data.statusCode != 200) {
        localStorage.removeItem('token')
        location.reload();
        token = undefined
        return;
      }
      const tableBody = document.getElementById('table-body')
      tableBody.innerHTML = ''
      data = data.body
      if (data.cars) {
        localStorage.setItem('token', token)
      }
      let i = 1
      data.cars.forEach(car => {
        const row = document.createElement('tr')
        row.innerHTML = `
          <td>${i++}</td>
          <td>${car.name}</td>
          <td>${car.totalIncome}</td>
          <td>${car.totalKwh}</td>
          <td>${car.totalOutcome}</td>
          <td>${car.totalProfit}</td>
          <td>${car.percentage}%</td>
          <td>${car.totalOurIncome}</td>
          <td>${car.totalInvestorsIncome}</td>
        `
        tableBody.appendChild(row)
      })
      const tableFooter = document.getElementById('table-footer')
      tableFooter.innerHTML = `
      <tr>
        <td rowspan="2" colspan="2">Итого</td>
        <td>Доход</td>
        <td>KWH * 10</td>
        <td>Расход</td>
        <td>Общая</td>
        <td>Машин</td>
        <td>Комиссия</td>
        <td>Прибыль</td>
      </tr>
      <tr>

        <td>${data.total.totalIncome}</td>
        <td>${data.total.totalKwh}</td>
        <td>${data.total.totalOutcome}</td>
        <td>${data.total.totalProfit}</td>
        <td>${data.total.totalCarsCount}</td>
        <td>${data.total.totalOurIncome}</td>
        <td>${data.total.totalInvestorsIncome}</td>
      </tr>
      `
    })
    .catch(error => console.error(error))
  document.body.style.cursor='default'
}

function showMenu(from, to) {
  if (token) {
    document.getElementById('menu').style.display = 'block'
    document.getElementById('events').style.display = 'none'
    document.getElementById('login').style.display = 'none'

    document.getElementById('start-date').valueAsDate = from
    document.getElementById('end-date').valueAsDate = to
  }
  else {
    document.getElementById('menu').style.display = 'none'
    document.getElementById('events').style.display = 'none'
    document.getElementById('login').style.display = 'block'
  }
}

function filterReport() {
  from = new Date(document.getElementById('start-date').value)
  to = new Date(document.getElementById('end-date').value)
  loadReport(from, to)
}

function filterEvents() {
  from = new Date(document.getElementById('events-start-date').value)
  to = new Date(document.getElementById('events-end-date').value)
  loadEvents(from, to)
}

// events menu
async function openEvents() {
  from = new Date(document.getElementById('start-date').value)
  to = new Date(document.getElementById('end-date').value)
  document.getElementById('events-start-date').value = from.toISOString().slice(0, 10)
  document.getElementById('events-end-date').value = to.toISOString().slice(0, 10)
  await loadEvents(from, to)
  showEvents()
}

async function loadEvents(from, to) {
  document.body.style.cursor='wait'
  await fetch('https://85wc02tqsc.execute-api.eu-north-1.amazonaws.com/ecotaxi-investor/getInvestorEvents', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify({ password: token, from: from.toISOString().slice(0, 10), to: to.toISOString().slice(0, 10) })
  })
    .then(response => {
      return response.json()
    })
    .then(data => {
      if (data.statusCode != 200) {
        localStorage.removeItem('token')
        location.reload();
        token = undefined
        return;
      }
      const tableBody = document.getElementById('events-table-body')
      tableBody.innerHTML = ''
      data = data.body
      let i = 1
      data.events.forEach(event => {
        const row = document.createElement('tr')
        row.innerHTML = `
          <td>${i++}</td>
          <td>${event.time}</td>
          <td>${event.typeName}</td>
          <td>${event.driverId}</td>
          <td>${event.carId}</td>
          <td>${event.amount}</td>
          <td>${event.description}</td>
        `
        tableBody.appendChild(row)
      })
      const tableFooter = document.getElementById('events-table-footer')
      tableFooter.innerHTML = `
      <tr>
        <td rowspan="2" colspan="3">Итого</td>
        <td>Доход</td>
        <td>Расход</td>
        <td colspan="2">Прибыль</td>
      </tr>
      <tr>
        <td >${data.total.income}</td>
        <td >${data.total.outcome}</td>
        <td colspan="2">${data.total.amount}</td>
      </tr>
      `
    })
    .catch(error => console.error(error))
  document.body.style.cursor='default'
}

function showEvents() {
  if (token) {
    document.getElementById('menu').style.display = 'none'
    document.getElementById('events').style.display = 'block'
    document.getElementById('login').style.display = 'none'
  }
  else {
    document.getElementById('menu').style.display = 'none'
    document.getElementById('events').style.display = 'none'
    document.getElementById('login').style.display = 'block'
  }
}

function logout() {
  localStorage.removeItem('token')
  token = undefined
  openMenu()
}
