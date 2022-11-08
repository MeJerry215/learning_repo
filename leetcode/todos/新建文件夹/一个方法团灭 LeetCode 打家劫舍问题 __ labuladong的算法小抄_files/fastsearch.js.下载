var fuse; // holds our search engine
var fuseIndex;
var searchVisible = false;
var firstRun = true; // allow us to delay loading json data unless search activated
var list = document.getElementById('mySearchResults'); // targets the <ul>
var first = null; // first child of search list
var last = null; // last child of search list
var maininput = document.getElementById('search-by'); // input box for search
var resultsAvailable = false; // Did we get any search results?

$(document).ready(function () {
  $('mySearchResults').blur(function () {
    console.log('mySearchResults blur')
    $('mySearchResults').hide();
  });
})

// ==========================================
// The main keyboard event listener running the show
//
document.addEventListener('keydown', function (event) {

  // CMD-/ to show / hide Search
  if (event.altKey && event.which === 191) {
    // Load json search index if first time invoking search
    // Means we don't load json unless searches are going to happen; keep user payload small unless needed
    doSearch(event)
  }

  // Allow ESC (27) to close search box
  if (event.keyCode == 27) {
    if (searchVisible) {
      document.getElementById("fastSearch").style.visibility = "hidden";
      document.activeElement.blur();
      searchVisible = false;
    }
  }

  // DOWN (40) arrow
  if (event.keyCode == 40) {
    if (searchVisible && resultsAvailable) {
      console.log("down");
      event.preventDefault(); // stop window from scrolling
      if (document.activeElement == maininput) { first.focus(); } // if the currently focused element is the main input --> focus the first <li>
      else if (document.activeElement == last) { last.focus(); } // if we're at the bottom, stay there
      else { document.activeElement.parentElement.nextSibling.firstElementChild.focus(); } // otherwise select the next search result
    }
  }

  // UP (38) arrow
  if (event.keyCode == 38) {
    if (searchVisible && resultsAvailable) {
      event.preventDefault(); // stop window from scrolling
      if (document.activeElement == maininput) { maininput.focus(); } // If we're in the input box, do nothing
      else if (document.activeElement == first) { maininput.focus(); } // If we're at the first item, go to input box
      else { document.activeElement.parentElement.previousSibling.firstElementChild.focus(); } // Otherwise, select the search result above the current active one
    }
  }
});


// ==========================================
// execute search as each character is typed
//
document.getElementById("search-by").onkeyup = function (e) {
  executeSearch(this.value);
}

document.querySelector("body").onclick = function (e) {
  if (e.target.tagName === 'BODY' || e.target.tagName === 'DIV') {
    hideSearch()
  }
}

document.querySelector("#search-by").onclick = function (e) {
  doSearch(e)
}

function doSearch(e) {
  e.stopPropagation();
  if (firstRun) {
    loadSearch() // loads our json data and builds fuse.js search index
    firstRun = false // let's never do this again
  }
  // Toggle visibility of search box
  if (!searchVisible) {
    showSearch() // search visible
  }
  else {
    hideSearch()
  }
}

function hideSearch() {
  // document.getElementById("fastSearch").style.visibility = "hidden" // hide search box
  // document.activeElement.blur() // remove focus from search box 
  // searchVisible = false
}

function showSearch() {
  // document.getElementById("fastSearch").style.visibility = "visible" // show search box
  // document.getElementById("search-by").focus() // put focus in input box so you can just start typing
  // searchVisible = true
}

// ==========================================
// fetch some json without jquery
//
function fetchJSONFile(path, callback) {
  var httpRequest = new XMLHttpRequest();
  httpRequest.onreadystatechange = function () {
    if (httpRequest.readyState === 4) {
      if (httpRequest.status === 200) {
        var data = JSON.parse(httpRequest.responseText);
        if (callback) callback(data);
      }
    }
  };
  httpRequest.open('GET', path);
  httpRequest.send();
}


// ==========================================
// load our search index, only executed once
// on first call of search box (CMD-/)
//
function loadSearch() {
  console.log('loadSearch()')
  fetchJSONFile('/algo/index.json', function (data) {

    var options = { // fuse.js options; check fuse.js website for details
      shouldSort: true,
      location: 0,
      distance: 100,
      threshold: 0.4,
      minMatchCharLength: 2,
      keys: [
        'source',
        'title',
        'tags',
        'content'
      ]
    };
    // Create the Fuse index
    fuseIndex = Fuse.createIndex(options.keys, data)
    fuse = new Fuse(data, options, fuseIndex); // build the index from the json file
  });
}


// ==========================================
// using the index we loaded on CMD-/, run 
// a search query (for "term") every time a letter is typed
// in the search box
//
function executeSearch(term) {
  let results = fuse.search(term); // the actual query being run using fuse.js
  console.log(results)
  let searchitems = ''; // our results bucket

  if (results.length === 0) { // no results based on what was typed into the input box
    resultsAvailable = false;
    searchitems = '';
  } else { // build our html
    // console.log(results)
    sources = [];
    numLimit = 5;
    for (let item in results) { // only show first 5 results
      if (item > numLimit) {
        break;
      }
      let uri = results[item].item.source
      uri = uri.split("https://labuladong.gitee.io/")[1]
      uri = location.protocol + '//' + location.host + "/algo/" + uri

      if (sources.includes(uri)) {
        continue;
      }
      console.log('item: %s, title: %s', item, results[item].item.title)
      searchitems = searchitems + '<li><a href="' + uri + '" tabindex="0">' + '<span class="title">' + results[item].item.title + '</span></a></li>';
      sources.push(uri);
    }
    resultsAvailable = true;
    $('mySearchResults').show();
  }

  document.getElementById("mySearchResults").innerHTML = searchitems;
  if (results.length > 0) {
    first = list.firstChild.firstElementChild; // first result container — used for checking against keyboard up/down location
    last = list.lastChild.firstElementChild; // last result container — used for checking against keyboard up/down location
  }
}
