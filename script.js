// Firebase Database URL with Authentication
const firebaseUrl =
  "https://macut-api-default-rtdb.firebaseio.com/data.json?AIzaSyCW39BT9fof5O0dq2s0rl5vbIyDnnomaz0Y"; // Replace with your Firebase secret or API key

// Function to fetch data from Firebase and display it in `file1-display` and `file2-display`
function fetchData() {
  fetch(firebaseUrl)
    .then((response) => response.json())
    .then((data) => {
      // Display content for file1 and file2
      const file1Text = data.file1 || "No data for File 1";
      const file2Text = data.file2 || "No data for File 2";

      document.getElementById("file1-display").innerText = file1Text;
      document.getElementById("file2-display").innerText = file2Text;
    })
    .catch((error) => console.error("Error fetching data:", error));
}

// Fetch data from Firebase when the page loads
window.onload = fetchData;

// Optional: You can also reload data every few seconds
setInterval(fetchData, 5000); // Fetch new data every 5 seconds
