// Firebase Database URL with Authentication
const firebaseUrl =
  "https://macut-api-default-rtdb.firebaseio.com/data.json?auth=AIzaSyCW39BT9fof5O0dq2s0rl5vbIyDnnomaz0Y"; // Replace with your Firebase secret or API key

// Variable to store the last fetched content of file2
let lastFile2Content = "";

// Function to fetch data from Firebase and display it for file2
function fetchData() {
  fetch(firebaseUrl)
    .then((response) => response.json())
    .then((data) => {
      // Get the content of file2 and put it inside the textarea
      const file2Text = data.file2 || "No content for File 2"; // Default text if file2 is empty

      document.getElementById("file2-input").value = file2Text;  // Set the content of file2 into the textarea

      // Store the fetched content to compare later
      lastFile2Content = file2Text;
    })
    .catch((error) => console.error("Error fetching data:", error));
}

// Fetch data from Firebase when the page loads (only once during setup)
window.onload = fetchData;

// Function to save edited file2 content to Firebase
function saveFile2() {
  const file2Content = document.getElementById("file2-input").value;

  // Only proceed if the content has changed
  if (file2Content !== lastFile2Content) {
    // Prepare payload for updating file2 content
    const payload = {
      file2: file2Content
    };

    // Send updated content to Firebase
    fetch(firebaseUrl, {
      method: "PATCH",
      headers: {
        "Content-Type": "application/json"
      },
      body: JSON.stringify(payload)
    })
      .then(response => response.json())
      .then(data => {
        console.log("File2 updated successfully:", data);
        lastFile2Content = file2Content;  // Update lastFile2Content to new content
      })
      .catch((error) => console.error("Error updating file2:", error));
  } else {
    console.log("No changes made to file2. Skipping update.");
  }
}
