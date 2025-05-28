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
  await fetch('/report?from=' + from.toISOString().slice(0, 10) + '&to=' + to.toISOString().slice(0, 10), {
    headers: {
      'token': encodeURIComponent(token)
    }
  })

    .then(response => {
      if (response.status != 200) {
        localStorage.removeItem('token')
        token = undefined
        return;
      }
      return response.json()
    })
    .then(data => {
      const tableBody = document.getElementById('table-body')
      tableBody.innerHTML = ''
      if (data.cars) {
        localStorage.setItem('token', token)
      }
      let i = 1
      data.cars.forEach(car => {
        const row = document.createElement('tr')
        row.innerHTML = `
          <td>${i++}</td>
          <td>${car.sid}</td>
          <td>${car.income}</td>
          <td>${car.outcome}</td>
          <td>${car.general}</td>
          <td>${car.percent}%</td>
          <td>${car.fee}</td>
          <td>${car.profit}</td>
        `
        tableBody.appendChild(row)
      })
      const tableFooter = document.getElementById('table-footer')
      tableFooter.innerHTML = `
        <td colspan="2">Итого</td>
        <td>${data.total.income}</td>
        <td>${data.total.outcome}</td>
        <td>${data.total.general}</td>
        <td>${data.total.cars}</td>
        <td>${data.total.fee}</td>
        <td>${data.total.profit}</td>
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

// events menu
async function openEvents() {
  from = document.getElementById('start-date').value
  to = document.getElementById('end-date').value
  document.getElementById('events-start-date').value = from
  document.getElementById('events-end-date').value = to
  await loadEvents(from, to)
  showEvents()
}

async function loadEvents(from, to) {
  document.body.style.cursor='wait'
  await fetch('/events?from=' + from + '&to=' + to, {
    headers: {
      'token': encodeURIComponent(token)
    }
  })
    .then(response => {
      if (response.status != 200) {
        localStorage.removeItem('token')
        token = undefined
        return;
      }
      return response.json()
    })
    .then(data => {
      const tableBody = document.getElementById('events-table-body')
      tableBody.innerHTML = ''
      let i = 1
      data.events.forEach(event => {
        const row = document.createElement('tr')
        row.innerHTML = `
          <td>${i++}</td>
          <td>${event.date}</td>
          <td>${event.type}</td>
          <td>${event.car}</td>
          <td>${event.driver}</td>
          <td>${event.amount}</td>
          <td>${event.description}</td>
        `
        tableBody.appendChild(row)
      })
      const tableFooter = document.getElementById('events-table-footer')
      tableFooter.innerHTML = `
        <td colspan="3">Итого</td>
        <td colspan="4">${data.total.amount}</td>
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
