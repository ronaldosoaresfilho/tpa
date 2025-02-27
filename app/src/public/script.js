document.getElementById("inputForm").addEventListener("submit", async function(event) {
    event.preventDefault();

    document.getElementById("output").value = "";

    let inputText = document.getElementById("userInput").value;

    let response = await fetch("/process", {
        method: "POST",
        headers: { "Content-Type": "application/x-www-form-urlencoded" },
        body: "user_input=" + encodeURIComponent(inputText)
    });

    let result = await response.text();
    document.getElementById("output").innerHTML = result.replace(/\n/g, "<br>");

    // Limpa o campo de entrada após o envio
    document.getElementById("userInput").value = "";
});
