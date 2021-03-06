// Copyright 2017 Hunter L. Allen
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __WORKER_NODE_HPP__
#define __WORKER_NODE_HPP__
#pragma once

/* Qt Includes */
#include <stdexcept>
#include <QtNetwork>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QtCore>

/* File Includes */
#include "user.hpp"
#include "tcp_comm.hpp"
#include "tcp_thread.hpp"
#include "event_struct.hpp"
#include "thread_init_exception.hpp"
#include "worker_connection_state.hpp"

class tcp_thread;

class worker_node : public QObject
{
  Q_OBJECT

public:
  explicit worker_node(
    const QString & _hostname,
    const quint16 & _port,
    QObject * _p_parent = NULL);
  virtual ~worker_node();

  bool init();

  bool try_login(
    const QString & _user,
    const QString & _password);
  bool try_create(
    const QString & _user,
    const QString & _password,
    const QString & _email);
  bool user_in_group(
    const QString & _user,
    const QString & _group);
  bool group_exists(const QString & _group);
  bool is_valid_for_user(
    const QString &,
    const calendar_event &);
  QSet<calendar_event> suggest_event_times(
    const QString &, const QString &,
    const QString &, const QString &);

  Q_SIGNAL void established_client_connection();
  Q_SIGNAL void finished_client_job();

  Q_SLOT void run();
  Q_SLOT void stop() {m_continue = false;}
  Q_SLOT void start_thread() {m_p_thread->start();}

  Q_SLOT QSqlDatabase setup_db();
  Q_SLOT bool insert_user(user & u);
  Q_SLOT bool select_user(user & u);
  Q_SLOT bool select_schedule_id(user & u);
  Q_SLOT bool insert_group(const QString & group_name);
  Q_SLOT bool join_group(
    const QString & user_name,
    const QString & group_name);
  Q_SLOT bool leave_group(
    const QString & user_name,
    const QString & group_name);
  Q_SLOT bool remove_group(const QString & group_name);
  Q_SLOT bool update_user(
    const QString & _old_user, const QString & _old_pass,
    const QString & _new_pass, const QString & _new_user,
    const QString & _new_mail, const QString & _new_cell);
  Q_SLOT bool list_groups(const QString &, QString *);
  Q_SLOT bool get_account_info(const QString &, QString *);
  Q_SLOT bool list_group_users(const QString &, QString *);
  Q_SLOT bool create_personal_event(
    const QString &, const QString &,
    const QString &, const QString &,
    const QString &, const QString &,
    const QString &, const QString &);
  Q_SLOT bool create_friendship(const QString &, const QString &);
  Q_SLOT bool accept_friend(const QString &, const QString &);
  Q_SLOT bool delete_friend(const QString &, const QString &);
  Q_SLOT bool friend_requests(const QString &, QString *);
  Q_SLOT bool friends(const QString &, QString *);
  Q_SLOT bool absent(const QString &);
  Q_SLOT bool present(const QString &);
  Q_SLOT bool list_user_events(
    const QString &,
    const QString &,
    const QString &,
    QString *);
  Q_SLOT bool list_user_month_events(
    const QString &,
    const quint8 &,
    const quint16 &,
    QString *);
  Q_SLOT bool suggest_user_events(
    const QString &,
    const QString &,
    const QString &,
    const QString &,
    QString * _msg);
  Q_SLOT bool suggest_group_events(
    const QString &,
    const QString &,
    const QString &,
    const QString &,
    QString * _msg);
  Q_SLOT void handle_client_disconnect();

  bool reset_password(
    QString & _p_user, QString & _p_email,
    QString & _p_new_psswd);

  bool username_exists(const QString & _user);
  bool cleanup_db_insert();
  bool cleanup_group_insert();
  bool cleanup_user_group_insert();

  void set_master_hostname(const QString & _master_host)
  {
    m_master_host = _master_host;
  }

  void set_master_port(const quint16 & _master_port)
  {
    m_master_port = _master_port;
  }

  Q_SIGNAL void disconnect_client(
    tcp_connection * client,
    QString * _p_msg);
  Q_SLOT void request_create_account(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_login(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_create_group(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_add_to_group(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_leave_group(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_update_user(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_user_groups(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_account_info(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_delete_group(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_group_users(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_personal_event(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_group_event(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_reset_password(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_user_events(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_group_events(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_personal_month_events(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_group_month_events(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_create_friendship(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_accept_friend(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_friends(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_delete_friend(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_friend_requests(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_absent(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_present(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_suggest_user_times(
    QString * _p_text,
    QTcpSocket * _p_socket);
  Q_SLOT void request_suggest_group_times(
    QString * _p_text,
    QTcpSocket * _p_socket);

private:
  volatile bool m_continue = true;

  QString m_host;
  quint16 m_port;

  QString m_master_host = "localhost";
  quint16 m_master_port = 3224;

  tcp_thread * m_p_tcp_thread;
  QThread * m_p_thread;

  connection_state state;       /* state enum for the state machine */

  quint16 sleep_time = 400;
  QSqlDatabase m_db;
  volatile bool served_client = false;
  QMutex * m_p_mutex;
};
#endif
