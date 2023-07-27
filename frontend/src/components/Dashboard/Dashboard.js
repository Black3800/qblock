import React, { useState } from "react"
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome"
import { faCircleInfo, faRotate, faKey, faPaperPlane } from "@fortawesome/free-solid-svg-icons";

import "./Dashboard.css"
import "./Block.css";

const Block = (props) => {
  const [hover, setHover] = useState(false)
  const [view, setView] = useState(false)

  function handleMouseover() {
    setHover(true)
  }

  function handleMouseout() {
    setHover(false)
  }

  function toggleView() {
    alert(`hash: ${block._id}
    message: ${block.message}
    message_hash: ${block.message_hash}
    previous_hash: ${block.previous_hash}
    previous_timestamp: ${block.previous_timestamp}
    proof: ${block.proof}
    public_key: ${block.public_key}
    signature: ${block.signature}
    timestamp: ${block.timestamp}`);
  }

  const block = props.block

  return (
    <div
      className="block"
      onMouseOver={handleMouseover}
      onMouseOut={handleMouseout}
      onClick={toggleView}
    >
      <div className="block-height-container">
        <div className="block-height">{block.height}</div>
      </div>
      <div className="block-message">
        {block.message}
      </div>
      {hover && (
        <FontAwesomeIcon icon={faCircleInfo} className="block-info-icon" />
      )}
    </div>
  );
}

const Dashboard = () => {
  let sk = ""
  let pk = ""

  const [loadKey, setLoadKey] = useState(false)

  function loadKeyBtn() {
    setLoadKey(true)
  }

  function unloadKeyBtn() {
    setLoadKey(false);
  }

  const [loadSubmit, setLoadSubmit] = useState(false);

  function loadSubmitBtn() {
    setLoadSubmit(true);
  }

  function unloadSubmitBtn() {
    setLoadSubmit(false);
  }

  function getKey() {
    if (loadKey) return
    loadKeyBtn()
    fetch("http://qblock.anakint.com:23614/key")
    .then(response => {
      return response.json();
    })
    .then(data => {
      sk = data.secret_key
      pk = data.public_key
      const encodedSk = sk.replace(/[\u00A0-\u9999<>\&]/g, i => '&#'+i.charCodeAt(0)+';')
      const encodedPk = pk.replace(/[\u00A0-\u9999<>\&]/g, i => '&#'+i.charCodeAt(0)+';')
      document.getElementById("sk").innerHTML = encodedSk
      document.getElementById("pk").innerHTML = encodedPk
      unloadKeyBtn()
    })
    .catch(error => {
      console.log("失敗しました");
    });
  }

  function submit() {
    if (loadSubmit) return;
    loadSubmitBtn();
    fetch("http://qblock.anakint.com:23614/cold_with_sk", {
      method: "post",
      headers: {
        Accept: "application/json",
        "Content-Type": "application/json",
      },
      body: JSON.stringify({
        message: document.getElementById("msg").value,
        secret_key: sk,
      }),
    })
      .then((response) => {
        return response.json();
      })
      .then((data) => {
        document.getElementById("msg").value = "";
        unloadSubmitBtn();
      })
      .catch((error) => {
        console.log("失敗しました");
      });
  }

  const [loadRefresh, setLoadRefresh] = useState(false);
  const [blocks, setBlocks] = useState([])

  function loadRefreshBtn() {
    setLoadRefresh(true);
  }

  function unloadRefreshBtn() {
    setLoadRefresh(false);
  }

  function refresh() {
    if (loadRefresh) return;
    loadRefreshBtn();
    fetch("http://qblock.anakint.com:23614/hot")
      .then((response) => {
        return response.json();
      })
      .then((data) => {
        setBlocks(data.blocks.reverse())
        unloadRefreshBtn();
      })
      .catch((error) => {
        console.log("失敗しました");
      });
  }

  return (
    <div className="dashboard">
      <div className="dashboard-log" id="dashboard-log">
        <div className="dashboard-log-heading">Current blocks</div>
        <div className="block-refresh-container">
          <div
            className={loadRefresh ? "block-refresh load" : "block-refresh"}
            onClick={refresh}
          >
            {loadRefresh ? "Refreshing..." : "Refresh"}
            <FontAwesomeIcon icon={faRotate} className="block-refresh-icon" />
          </div>
        </div>
        <div className="block-container" id="block-container">
          {blocks.map((b) => (
            <Block block={b} key={b._id} />
          ))}
        </div>
      </div>
      <div className="dashboard-action" id="dashboard-action">
        <div className="dashboard-action-heading">Submit blocks</div>
        <div className="dashboard-action-panel">
          <div className="dashboard-action-panel-top">
            <div className="key-container">
              <div className="key" id="sk"></div>
              <div className="key" id="pk"></div>
            </div>
            <div className="key-button-container">
              <div
                className={loadKey ? "key-button load" : "key-button"}
                onClick={getKey}
              >
                {loadKey ? "Generating..." : "Generate a new key pair"}
                <FontAwesomeIcon icon={faKey} className="key-button-icon" />
              </div>
            </div>
          </div>
          <div className="dashboard-action-panel-bottom">
            <label for="msg">
              <div>Message</div>
              <textarea
                className="msg"
                id="msg"
                placeholder="Type message here..."
              ></textarea>
            </label>
            <div className="msg-button-container">
              <div
                className={loadSubmit ? "msg-button load" : "msg-button"}
                onClick={submit}
              >
                {loadSubmit ? "Submitting..." : "Submit"}
                <FontAwesomeIcon
                  icon={faPaperPlane}
                  className="msg-button-icon"
                />
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
}

export default Dashboard